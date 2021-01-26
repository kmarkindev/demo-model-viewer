#pragma once

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include "Exceptions.h"
#include "Settings.h"
#include "InputParams.h"

extern InputParams g_inputParams;

class Application 
{
private:
	std::string m_modelPath = "";
	GLFWwindow* m_window = nullptr;
	GLFWmonitor* m_monitor = nullptr;
	bool m_isInitialized = false;

	void CheckInitialization();

	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:

	Application(std::string modelPath);
	void Init();
	void Start();
	void Shutdown();
};