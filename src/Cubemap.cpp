#include "Cubemap.h"

Cubemap::Cubemap(GLuint cubemapId) 
{
    m_id = cubemapId;
}

GLuint Cubemap::GetTextureId() 
{
    return m_id;
}

void Cubemap::Unload() 
{
    if(m_id != 0)
    {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}
