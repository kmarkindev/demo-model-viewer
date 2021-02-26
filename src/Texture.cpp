#include "Texture.h"

void Texture::Unload() 
{
    if(id != 0)
    {
        glDeleteTextures(1, &id);
        id = 0;
    }
}
