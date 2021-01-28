#include "Model.h"

Model::Model(std::vector<Mesh>* meshes)
	: m_meshes(meshes)
{
}

Model::~Model()
{
	delete m_meshes;
}

void Model::SetShader(Shader* shader)
{
	m_shader = shader;
}
