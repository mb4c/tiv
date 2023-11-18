#pragma once

#include <string>
#include <glm/glm.hpp>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <memory>
#include "Input.hpp"


class Application
{
public:
	Application(const std::string& title, int width, int height, bool vsync);
	void Run();

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnShutdown() = 0;
	virtual void OnResize() = 0;

	GLFWwindow* GetWindow();
	[[nodiscard]] glm::ivec2 GetWindowSize() const { return {m_Width, m_Height};}
	void SetWindowSize(glm::ivec2 size);
	Input m_Input{nullptr};
	AppData m_Appdata;

private:
	std::string m_Title;
	int m_Width;
	int m_Height;
	int m_PreviousWidth{};
	int m_PreviousHeight{};

	bool m_Vsync;
	GLFWwindow* m_Window{};

	static void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
};

