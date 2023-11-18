#include "Application.hpp"

Application::Application(const std::string& title, int width, int height, bool vsync)
{
	m_Title = title;
	m_Width = width;
	m_Height = height;
	m_Vsync = vsync;
	m_Appdata.app = this;
}

void Application::Run()
{
	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

	if (m_Window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return;
	}

	glfwSetWindowUserPointer(m_Window, &m_Appdata);

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
	{
		auto* data = static_cast<AppData*>(glfwGetWindowUserPointer(window));

		data->app->m_Width = width;
		data->app->m_Height = height;
		glViewport(0, 0, width, height);

//		std::cout << "Resized " << data->m_Width << " x " << data->m_Height << std::endl;
	});


	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return;
	}

	std::cout << "OpenGL version: " << *glGetString(GL_VERSION) << std::endl;
	glfwSwapInterval(m_Vsync);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, nullptr);

	m_Input = Input(m_Window);
	m_Appdata.input = &m_Input;

	glfwSetKeyCallback(GetWindow(), Input::KeyCallback);
	glfwSetScrollCallback(GetWindow(), Input::ScrollCallback);

	OnInit();

	while (!glfwWindowShouldClose(m_Window))
	{
		if (m_PreviousHeight != m_Height || m_PreviousWidth != m_Width)
		{
			OnResize();
		}

		OnUpdate();
		m_PreviousWidth = m_Width;
		m_PreviousHeight = m_Height;

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	OnShutdown();
	glfwDestroyWindow(m_Window);
	glfwTerminate();

}

GLFWwindow* Application::GetWindow()
{
	return m_Window;
}

void Application::SetWindowSize(glm::ivec2 size)
{
	glfwSetWindowSize(m_Window, size.x, size.y);
	glViewport(0, 0, size.x, size.y);
}

void Application::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_param)
{
	auto const src_str = [source]()
	{
		switch (source)
		{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			default: return "DEFAULT";
		}
	}();

	auto const type_str = [type]()
	{
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			case GL_DEBUG_TYPE_OTHER: return "OTHER";
			default: return "DEFAULT";

		}
	}();

	auto const severity_str = [severity]()
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "DEFAULT";

		}
	}();
	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}