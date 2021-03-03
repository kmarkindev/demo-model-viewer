#include "Model.h"

Model::Model(std::vector<Mesh>* meshes)
	: m_meshes(meshes)
{
}

Model::~Model()
{
	delete m_meshes;
}

Material* Model::GetMaterial() 
{
	return m_material;
}

void Model::SetMaterial(Material* material)
{
	m_material = material;
}

void Model::SetShader(Shader* shader)
{
	m_shader = shader;
}

void Model::UseOpacity(bool status) 
{
	m_useOpacity = status;
}

bool Model::GetUseOpacity() 
{
	return m_useOpacity;
}

void Model::UseNormal(bool status) 
{
	m_useNormal = status;
}

bool Model::GetUseNormal() 
{
	return m_useNormal;
}

void Model::Unload() 
{
	// m_material->UnloadMaterial();

	// for(auto mesh : m_meshes[0])
	// {
	// 	mesh.Unload();
	// }

	// delete m_meshes;
	// delete m_material;
}
