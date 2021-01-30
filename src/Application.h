#pragma once

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Exceptions.h"
#include "Config.h"
#include "AssimpLoader.h"
#include "Camera.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>

extern Config g_config;

class Application 
{
private:
	std::string m_modelPath = "";
	GLFWwindow* m_window = nullptr;
	GLFWmonitor* m_monitor = nullptr;
	bool m_isInitialized = false;
	AssimpLoader* m_loader = nullptr;
	Renderer* m_renderer = nullptr;

	void CheckInitialization();

	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:

	Application(std::string modelPath);
	void Init();
	void Start();
	void Shutdown();
};