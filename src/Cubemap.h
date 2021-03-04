#pragma once

#include <glad/glad.h>

class Cubemap
{
public:

    Cubemap(GLuint cubemapId);

    GLuint GetTextureId();
    void Unload();

private:

    GLuint m_id;

};