#pragma once

#include "Mesh.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

class AssimpLoader
{
public:

	AssimpLoader(unsigned int importFlags);

	Model LoadModel(std::string path);

private:

	Assimp::Importer m_importer;
	unsigned int m_importFlags;

	Mesh ProcessMesh(aiMesh* aiMesh, const aiScene* aiScene);

};