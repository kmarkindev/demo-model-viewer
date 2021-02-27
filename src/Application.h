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
#include <filesystem>
#include "TextureLoader.h"
#include "AssetsManager.h"
#include <ImGuiFileDialog.h>

class Application 
{
private:

	GLFWwindow* m_window = nullptr;
	GLFWmonitor* m_monitor = nullptr;
	bool m_isInitialized = false;
	AssimpLoader* m_loader = nullptr;
	TextureLoader* m_textureLoader = nullptr;
	Renderer* m_renderer = nullptr;
	AssetsManager* m_assetsManager = nullptr;
	ImGuiIO* m_imguiIo;

	Camera* m_camera = nullptr;
	Model* m_model = nullptr;
	DirLight* m_light = nullptr;
	Shader* m_shader = nullptr;

	glm::vec3 m_startCameraPosition = glm::vec3(0.f, 100.f, 0.f);
	glm::vec4 m_startLightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 m_startModelScale = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 m_startLightDirection = glm::normalize(glm::vec3(1.0, 0.f, 1.0f));

	int m_startWidth = 900;
	int m_startHeight = 900;
	float m_startFov = 90;
	float m_startNear = 0.1f;
	float m_startFar = 300.f;
	bool m_isFullscreenOnStart = false;
	std::string m_startWindowTitle = "DEMO: Model Viewer";
	float m_startSensivitity = 0.3f;
	float m_startScrollSensivitity = 1.f;

	bool m_shouldRotate = false;

	float m_menuAlpha = 1.f;

	void CheckInitialization();
	
	void SetupShader();
	void LoadModel(std::string path);
	void LoadTexture(std::string path, TextureType type);
	void SetupCamera();
	void SetupLight();

	void UnloadCurrentModel();

	void InitImgui();
	void ShutdownImgui();
	void StartImguiFrame();
	void EndImgueFrame();
	void DrawImguiUi();

	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void WindowSizeCallback(GLFWwindow* window, int width, int height);
	static Application* GetInstancePointer(GLFWwindow* window);

public:
	
	void Init(int argc, char* argv[]);
	void Start();
	void Shutdown();
};