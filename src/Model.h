#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include <vector>

class Renderer;

class Model : public Object
{
private:

	friend class Renderer;

	Shader* m_shader;
	std::vector<Mesh>* m_meshes;

public:

	Model(std::vector<Mesh>* meshesm);
	~Model();

	void SetShader(Shader* shader);

};