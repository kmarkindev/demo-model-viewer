#include "Material.h"

void Material::UnloadMaterial() 
{
    specular.Unload();
    diffuse.Unload();
    opacity.Unload();
    normal.Unload();
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
    case TextureType::Opacity:
        texture = &opacity;
        break;
    case TextureType::Normal:
        texture = &normal;
        break;
    default:
        throw NotImplementedException();
    }

    texture->Unload();
    *texture = newTexture;
}
