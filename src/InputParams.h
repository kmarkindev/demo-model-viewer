#pragma once

#include <string>

class InputParams
{
public:
	std::string rootFolder;
	std::string modelPath;

	std::string GetModelPath();
};