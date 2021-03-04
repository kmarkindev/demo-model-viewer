#include "Cubemap.h"

Cubemap::Cubemap(GLuint cubemapId) 
{
    m_id = cubemapId;
}

GLuint Cubemap::GetTextureId() 
{
    return m_id;
}
