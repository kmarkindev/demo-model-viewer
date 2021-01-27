#pragma once

#include "Object.h"
#include "Mesh.h"
#include <vector>

class Renderer;

class Model : public Object
{
private:

	friend class Renderer;

	std::vector<Mesh>* m_meshes;

public:

	Model(std::vector<Mesh>* meshes);
	~Model();

};