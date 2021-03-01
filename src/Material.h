#pragma once

#include "Texture.h"
#include "glad/glad.h"
#include "Exceptions.h"

struct Material
{
	Texture diffuse;
	Texture specular;
	Texture opacity;

	void UnloadMaterial();
	void ReplaceTexture(Texture newTexture, TextureType type);
};