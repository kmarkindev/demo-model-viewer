#pragma once

#include <glad/glad.h>
#include "Shader.h"
#include "Cubemap.h"

class Renderer;

class Skybox
{

    friend class Renderer;

    Cubemap m_cubemap;
    Shader* m_shader;

    GLuint m_VAO;
    GLuint m_VBO;

public:

    Skybox(Cubemap cubemap, Shader* shader);

    void Unload();

private: 

    float* m_vertices = nullptr;

};