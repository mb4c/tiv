#pragma once

#include <memory>
#include "Application.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "Tile.hpp"
#include "Input.hpp"
#include <chrono>
#include <algorithm>
class ImageViewer : public Application
{
public:
	ImageViewer(const std::string &title, int width, int height, bool vsync, std::vector<std::string> args);
private:
	void OnInit() override;
	void OnUpdate() override;
	void OnShutdown() override;
	void OnResize() override;
	static void DropCallback(GLFWwindow* window, int count, const char** paths);

	std::vector<std::string> m_Args;

	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Shader> m_BgShader;

	ImageBuf m_LoadedImage;
	ImageBuf m_CheckerImage;
	Texture m_CheckerTexture;

	std::string m_LoadedImagePath;

	std::vector<Tile> m_Tiles;
	Tile m_BackgroundTile;

	int m_TileWidth = 1024 * 8;
	int m_TileHeight = 1024 * 8;

	int m_NumTilesX = 0;
	int m_NumTilesY = 0;

	float m_CurrentZoom = 1;
	float m_Zoom = 1;
	float m_panX{};
	float m_panY{};

	void LoadImage(const std::string& path);
	void UnloadImage();

	void PanAndZoom();
	void RenderTiles();

	void GenerateBackground();
	static ImageBuf GenerateCheckerImage(int width, int height);

	static float Normalize(float input, float min, float max);
	static float EaseOutQuint(float x);

	glm::mat4 m_View = glm::mat4(1.0f);
	glm::mat4 m_Projection{};
};
