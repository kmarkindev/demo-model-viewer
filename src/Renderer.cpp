#include "Renderer.h"

void Renderer::Init()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::Draw(Model* model, Shader* shader, Camera* camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for each (Mesh mesh in model->m_meshes[0])
	{
		glBindVertexArray(mesh.m_meshData.vao);
		glDrawElements(GL_TRIANGLES, mesh.m_meshData.dataLength, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void Renderer::Shutdown()
{
}
