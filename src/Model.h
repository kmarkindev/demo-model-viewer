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
	bool m_useOpacity = false;
	bool m_useNormal = false;

public:

	Model(std::vector<Mesh>* meshesm);
	~Model();

	Material* GetMaterial();
	void SetMaterial(Material* material);
	void SetShader(Shader* shader);

	void UseOpacity(bool status);
	bool GetUseOpacity();

	void UseNormal(bool status);
	bool GetUseNormal();

	void Unload();

};