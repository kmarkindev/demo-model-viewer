#include "TextureLoader.h"

Texture TextureLoader::LoadTexture(std::string path, TextureType type)
{
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);

	if (data == nullptr)
	{
		throw CannotLoadTexture();
	}

	GLuint id;

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

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

Cubemap TextureLoader::LoadCubemap(std::vector<std::string> paths) 
{
	if(paths.size() != 5)
	{
		throw CannotLoadCubemap();
	}

	GLuint id;
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width, height, channels;
	unsigned char* data = nullptr;

	for(int i = 0; i < 5; i++)
	{
		data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return Cubemap(id);
}