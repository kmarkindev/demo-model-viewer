#pragma once

#include <glad/glad.h>

enum class TextureType
{
	Diffuse,
	Specular,
	Opacity
};

struct Texture
{
	TextureType type;
	GLuint id;

	void Unload();
};