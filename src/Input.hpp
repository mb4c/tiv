#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include "Appdata.hpp"

class Input
{
public:
	explicit Input(GLFWwindow* window);

	bool GetKeyDown(int key);
	bool GetKeyPressed(int key);
	bool GetKeyReleased(int key);
	bool GetMouseDown(int button);
	bool GetMouseUp(int button);
	glm::vec2 GetMousePos();
	void SetMousePos(glm::vec2 pos);
	glm::vec2 GetMouseDelta();
	double GetScrollDelta();
	void UpdateMouseDelta();
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void UpdateKeys();

private:
	GLFWwindow* m_Window = nullptr;
	glm::vec2 m_LastCursorPos = glm::vec2(0,0);
	glm::vec2 m_CursorDelta = glm::vec2(0,0);
	bool m_FirstUpdate = true;
	double m_ScrollDelta = 0.0;

	std::map<int, bool> m_Keys {};
	std::map<int, bool> m_LastKeys {};
};
