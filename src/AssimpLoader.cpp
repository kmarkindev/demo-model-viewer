#include "AssimpLoader.h"

AssimpLoader::AssimpLoader(unsigned int importFlags)
    : m_importFlags(importFlags)
{
}

Model* AssimpLoader::LoadModel(std::string path)
{
    const aiScene* scene = m_importer.ReadFile(path, m_importFlags);
    std::vector<Mesh>* meshes = new std::vector<Mesh>();

    for (size_t i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* aiMesh = scene->mMeshes[i];
        Mesh mesh = ProcessMesh(aiMesh, scene);
        meshes->push_back(mesh);
    }

    return new Model(meshes);
}

Mesh AssimpLoader::ProcessMesh(aiMesh* aiMesh, const aiScene* aiScene)
{
	std::vector<Vertex>* vertices = new std::vector<Vertex>();
	std::vector<unsigned int>* indices = new std::vector<unsigned int>();

    for (size_t j = 0; j < aiMesh->mNumVertices; j++)
    {
        auto aiPosition = aiMesh->mVertices[j];
        auto aiNormal = aiMesh->mNormals[j];
        auto aiTextureCoords = aiMesh->mTextureCoords[0][j];

        vertices->push_back({
            glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z),
            glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z),
            glm::vec2(aiTextureCoords.x, aiTextureCoords.y)
        });
    }

    for (size_t j = 0; j < aiMesh->mNumFaces; j++)
    {
        auto aiFace = aiMesh->mFaces[j];

        for (size_t k = 0; k < aiFace.mNumIndices; k++)
        {
            auto index = aiFace.mIndices[k];
            indices->push_back(index);
        }
    }

	GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &vertices->front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), &indices->front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoords));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	MeshData meshData = MeshData(VAO, VBO, EBO, indices->size());

	delete vertices;
	delete indices;

	return Mesh(meshData);
}
