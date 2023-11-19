#include "ImageViewer.hpp"
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/imagebuf.h>

#include <utility>
using namespace OIIO;

ImageViewer::ImageViewer(const std::string &title, int width, int height, bool vsync, std::vector<std::string> args)
		: Application(title, width, height, vsync), m_Args(std::move(args))
{
}


void ImageViewer::OnInit()
{
	OIIO::attribute("threads", 0); // Use as many threads as available


	const char* vertexShaderSource = R"(
	#version 460 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTexCoord;

	out vec2 TexCoord;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0);

		TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	}
)";

	const char* bgVertexShaderSource = R"(
	#version 460 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTexCoord;

	out vec2 TexCoord;

	const vec2 vert[4] = vec2[]
	(
			vec2(1.0f,  1.0f),
			vec2(1.0f, -1.0f),
			vec2(-1.0f, -1.0f),
			vec2(-1.0f,  1.0f)
	);

	void main()
	{
		gl_Position = vec4(vert[gl_VertexID], 0.1, 1.0);
		TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	}
)";

	const char* fragmentShaderSource = R"(
	#version 460 core
	out vec4 FragColor;

	in vec2 TexCoord;

	layout(binding = 0) uniform sampler2D texture0;

	void main()
	{
		FragColor = texture(texture0, TexCoord);
	}
)";

	int maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	std::cout << "Max texture size: " << maxTextureSize << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetDropCallback(GetWindow(), DropCallback);


	m_Shader = std::make_shared<Shader>(vertexShaderSource, fragmentShaderSource);
	m_BgShader = std::make_shared<Shader>(bgVertexShaderSource, fragmentShaderSource);

	m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -3.0f));

	if (m_Args.size() > 1)
	{
		std::cout << m_Args.at(1) << std::endl;
		LoadImage(m_Args.at(1));
	}
}

void ImageViewer::OnUpdate()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_Input.UpdateMouseDelta();

	if (m_LoadedImagePath != m_Appdata.dropPath)
	{
		m_LoadedImagePath = m_Appdata.dropPath;
		LoadImage(m_LoadedImagePath);
	}

	PanAndZoom();
	RenderTiles();

	m_Input.UpdateKeys();
}

void ImageViewer::OnShutdown()
{

}

void ImageViewer::OnResize()
{
	m_CheckerImage = GenerateCheckerImage(GetWindowSize().x, GetWindowSize().y);
	GenerateBackground();
}

void ImageViewer::PanAndZoom()
{
	m_Zoom += -(float)m_Input.GetScrollDelta();
	m_Zoom = glm::clamp(m_Zoom, 0.01f, 10.0f);

	m_CurrentZoom = lerp(m_CurrentZoom, m_Zoom, 0.2f);

	if (m_Input.GetMouseDown(0))
	{

		float x = m_Input.GetMouseDelta().x / (float)GetWindowSize().x;
		float y = m_Input.GetMouseDelta().y / (float)GetWindowSize().y;

		float aspect = (float)GetWindowSize().x / (float)GetWindowSize().y;

		m_panX = x * m_CurrentZoom * aspect * 2;
		m_panY = y * m_CurrentZoom * 2;

		m_View = glm::translate(m_View, {m_panX, m_panY, 0});
	}
}

void ImageViewer::RenderTiles()
{

	m_BgShader->Bind();
	m_BackgroundTile.Draw();


	for (auto & tile : m_Tiles)
	{
		m_Shader->Bind();

		m_Shader->SetMat4("view", m_View);
		float aspect = (float)GetWindowSize().x / (float)GetWindowSize().y;
		m_Projection = glm::ortho(-m_CurrentZoom * aspect, m_CurrentZoom * aspect, -m_CurrentZoom, m_CurrentZoom, 0.1f, 100.0f);
		m_Shader->SetMat4("projection", m_Projection);
		tile.Draw();
	}
}

void ImageViewer::LoadImage(const std::string& path)
{
	auto beg = std::chrono::high_resolution_clock::now();

	UnloadImage();

	std::cout << "Loading image" << std::endl;
	m_LoadedImage = ImageBuf(path);
	std::cout << "Flipping image" << std::endl;
	ImageBufAlgo::flip(m_LoadedImage, m_LoadedImage);
	std::cout << "Image loaded and flipped" << std::endl;

	m_LoadedImage = m_LoadedImage.copy(TypeDesc::UCHAR);

	// calculate num tiles
	m_NumTilesX = (m_LoadedImage.spec().width + m_TileWidth - 1) / m_TileWidth;
	m_NumTilesY = (m_LoadedImage.spec().height + m_TileHeight - 1) / m_TileHeight;

	std::cout << "Generating tiles" << std::endl;
	for (int y = 0; y < m_NumTilesY; ++y)
	{
		for (int x = 0; x < m_NumTilesX; ++x)
		{
			int tileX = x * m_TileWidth;
			int tileY = y * m_TileHeight;

			ImageBuf outTile;

			int tileWidthActual = std::min(m_TileWidth, m_LoadedImage.spec().width - tileX);
			int tileHeightActual = std::min(m_TileHeight, m_LoadedImage.spec().height - tileY);


			ROI roi(tileX, tileX + m_TileWidth, tileY, tileY + m_TileHeight);
			ROI roiActual(0, tileWidthActual, 0, tileHeightActual);
			ImageBufAlgo::cut(outTile, m_LoadedImage, roi);

			if (outTile.nchannels() == 3)
			{
				ImageBuf alpha;
				alpha.reset(OIIO::ImageSpec(outTile.spec().width, outTile.spec().height, 1, OIIO::TypeDesc::UCHAR));
				OIIO::ImageBufAlgo::zero(alpha);
				ImageBufAlgo::fill(alpha, 255, roiActual);
				outTile = ImageBufAlgo::channel_append(outTile, alpha);
			}


			Texture tex(outTile);
			Tile tile(tex, m_Shader);
			tile.transform = glm::translate(tile.transform, {x, y, 0.0f});
			m_Tiles.push_back(tile);
			std::cout << "Tile " << x << " x " << y << " generated" << std::endl;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
	std::cout << "Image loaded in: " << duration.count() << "ms"<< std::endl;
	std::cout << "Image size: " << m_LoadedImage.spec().width << " x " << m_LoadedImage.spec().height << std::endl;
	std::cout << "Tile size: " << m_TileWidth << " x " << m_TileHeight << std::endl;

	m_LoadedImage.reset(OIIO::ImageSpec(256, 256, 1, OIIO::TypeDesc::UCHAR)); // Free loaded image because we don't need it anymore
}

void ImageViewer::DropCallback(GLFWwindow* window, int count, const char** paths)
{
	auto* data = static_cast<AppData*>(glfwGetWindowUserPointer(window));
	data->dropPath = paths[0];
}

void ImageViewer::UnloadImage()
{
	for (auto & tile : m_Tiles)
	{
		glDeleteTextures(1, &tile.m_Texture.m_TextureID);
	}

	m_Tiles.clear();
}

ImageBuf ImageViewer::GenerateCheckerImage(int width, int height)
{
	ImageBuf buf(ImageSpec(width, height, 4, TypeDesc::UCHAR));
	float dark[4] = {0.176, 0.176, 0.176, 1};
	float light[4] = {0.356, 0.356, 0.356, 1};
	ImageBufAlgo::checker(buf, 32, 32, 1, dark, light, 0, 0, 0);
	return buf;
}

void ImageViewer::GenerateBackground()
{
	if (m_CheckerTexture.m_TextureID)
	{
		glDeleteTextures(1, &m_CheckerTexture.m_TextureID);
	}

	m_CheckerTexture = Texture(m_CheckerImage);
	m_BackgroundTile = Tile(m_CheckerTexture, m_BgShader);
	m_BackgroundTile.transform = glm::translate(m_BackgroundTile.transform, {0,0,0});
}