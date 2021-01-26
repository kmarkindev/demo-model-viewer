#pragma once

#include <string>

class Config
{
public:

	std::string rootFolder;
	std::string modelPath;
	std::string windowTitle = "DEMO: model viewer";

	struct ViewportSettings {
		bool fullScreen = false;
		int width = 800;
		int height = 600;
	} viewportSettings;

	std::string GetModelPath();

};