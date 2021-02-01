#pragma once

#include <string>
#include <array>
#include "Texture.h"
#include "stb_image.h"
#include "Exceptions.h"

class TextureLoader
{

public:

	Texture LoadTexture(std::string path, TextureType type);

};