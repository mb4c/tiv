#pragma once
#include <string>

class Application;
class Input;

struct AppData
{
	Application* app;
	Input* input;
	std::string dropPath;
};