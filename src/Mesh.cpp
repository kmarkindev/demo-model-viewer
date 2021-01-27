#include "Mesh.h"

MeshData::MeshData(GLuint vao, GLuint vbo, GLuint ebo, unsigned int dataLength)
	: vao(vao), vbo(vbo), ebo(ebo), dataLength(dataLength)
{
}

Mesh::Mesh(MeshData meshData)
	: m_meshData(meshData)
{
}