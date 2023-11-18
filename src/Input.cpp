#include <iostream>
#include "Input.hpp"

Input::Input(GLFWwindow* window)
{
	m_Window = window;
}

bool Input::GetKeyDown(int key)
{
	return m_Keys[key] > 0;
}

bool Input::GetKeyPressed(int key)
{
	return m_Keys[key] > 0 && m_LastKeys[key] == 0;
}

bool Input::GetKeyReleased(int key)
{
	return m_Keys[key] == 0 && m_LastKeys[key] > 0;
}

bool Input::GetMouseDown(int button)
{
	if(glfwGetMouseButton(m_Window, button) == GLFW_PRESS)
		return true;
	else
		return false;
}

bool Input::GetMouseUp(int button)
{
	if(glfwGetMouseButton(m_Window, button) == GLFW_RELEASE)
		return true;
	else
		return false;
}

glm::vec2 Input::GetMousePos()
{
	double x, y;
	glfwGetCursorPos(m_Window, &x, &y);
	return glm::vec2{x, y};
}

void Input::SetMousePos(glm::vec2 pos)
{
	glfwSetCursorPos(m_Window, pos.x, pos.y);
}

glm::vec2 Input::GetMouseDelta()
{
	return m_CursorDelta;
}

double Input::GetScrollDelta()
{
	return m_ScrollDelta;
}

void Input::UpdateMouseDelta()
{
	if (m_FirstUpdate)
	{
		m_LastCursorPos = GetMousePos();
		m_FirstUpdate = false;
	}

	m_CursorDelta.x = GetMousePos().x - m_LastCursorPos.x;
	m_CursorDelta.y = m_LastCursorPos.y - GetMousePos().y;

	m_LastCursorPos = GetMousePos();

}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto* input = (AppData*)glfwGetWindowUserPointer(window);

	if (action)
	{
		input->input->m_Keys[key] = true;
	} else
	{
		input->input->m_Keys[key] = false;
	}
}

void Input::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	// NOTE: smooth scrolling is not implemented for X11
	// https://github.com/glfw/glfw/issues/1376

	auto* input = (AppData*)glfwGetWindowUserPointer(window);

	input->input->m_ScrollDelta = yoffset / 10;

}


void Input::UpdateKeys()
{
	m_LastKeys = m_Keys;
	m_ScrollDelta = 0;
}


