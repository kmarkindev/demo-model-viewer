#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include <vector>

class Renderer;

class Model : public Object
{
private:

	friend class Renderer;

	Shader* m_shader;
	Material* m_material;
	std::vector<Mesh>* m_meshes;

public:

	Model(std::vector<Mesh>* meshesm);
	~Model();

	void SetMaterial(Material* material);
	void SetShader(Shader* shader);

};