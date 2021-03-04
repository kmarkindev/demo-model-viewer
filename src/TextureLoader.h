#pragma once

#include <string>
#include <array>
#include "Texture.h"
#include "stb_image.h"
#include "Exceptions.h"
#include "Cubemap.h"
#include <vector>

class TextureLoader
{

public:

	Texture LoadTexture(std::string path, TextureType type);

	/**
	 * paths order: 0 = +X, 1 = -X, 2 = +Y, 3 = -Y, 4 = +Z, 5 = -Z
	 */
	Cubemap LoadCubemap(std::vector<std::string> paths);

};