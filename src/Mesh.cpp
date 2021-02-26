#include "Mesh.h"

MeshData::MeshData(GLuint vao, GLuint vbo, GLuint ebo, unsigned int dataLength)
	: vao(vao), vbo(vbo), ebo(ebo), dataLength(dataLength)
{
}

Mesh::Mesh(MeshData meshData)
	: m_meshData(meshData)
{
}

void Mesh::Unload() 
{
	if(m_meshData.ebo != 0)
		glDeleteBuffers(1, &m_meshData.ebo);

	if(m_meshData.vao != 0)
		glDeleteBuffers(1, &m_meshData.vao);
	
	if(m_meshData.vbo != 0)
		glDeleteBuffers(1, &m_meshData.vbo);
}