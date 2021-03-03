#pragma once

#include <glad/glad.h>

enum class TextureType
{
	Diffuse,
	Specular,
	Opacity,
	Normal
};

struct Texture
{
	TextureType type;
	GLuint id;

	void Unload();
};