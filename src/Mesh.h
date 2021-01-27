#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

struct MeshData 
{
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	unsigned int dataLength;

	MeshData(GLuint vao, GLuint vbo, GLuint ebo, unsigned int dataLength);
};

class Mesh
{
private:

	friend class Renderer;

	MeshData m_meshData;

public:

	Mesh(MeshData meshData);

};