#pragma once

#include <glad/glad.h>

enum class TextureType
{
	Diffuse,
	Specular
};

struct Texture
{
	TextureType type;
	GLuint id;
};