#include "Material.h"

void Material::UnloadMaterial() 
{
    specular.Unload();
    diffuse.Unload();
}

void Material::ReplaceTexture(Texture newTexture, TextureType type) 
{
    Texture* texture = nullptr;
    switch (type)
    {
    case TextureType::Diffuse:
        texture = &diffuse;
        break;
    case TextureType::Specular:
        texture = &specular;
        break;
    default:
        throw NotImplementedException();
    }

    texture->Unload();
    *texture = newTexture;
}
