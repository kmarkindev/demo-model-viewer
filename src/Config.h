#pragma once

#include <string>

class Config
{
public:

	std::string rootFolder;
	std::string modelPath;
	std::string modelFolder;
	std::string windowTitle = "DEMO: model viewer";

	struct Textures {
		std::string diffuseName;
		std::string specularName;	
	} textures;

	struct Camera {
		float fov = 90;
		float near = 0.1f;
		float far = 100.f;
	} camera;

	struct Viewport {
		bool fullScreen = false;
		int width = 900;
		int height = 900;
	} viewport;

	float sensivitity = 0.1f;
	float scrollSensivitity = 0.1f;

	std::string GetModelPath();

};