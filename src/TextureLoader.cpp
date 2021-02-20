#include "TextureLoader.h"

Texture TextureLoader::LoadTexture(std::string path, TextureType type)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (data == nullptr)
	{
		throw CannotLoadTexture();
	}

	GLuint id;

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	if(channels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if(channels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(id);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	
	Texture texture;
	texture.id = id;
	texture.type = type;

	return texture;
}