#include "material.h"



materials::Material::Material(std::shared_ptr<radiance::textures::Texture> texture):
_texture{texture}
{

}

void materials::Material::setTexture(std::shared_ptr<radiance::textures::Texture> texture)
{
    _texture = texture;
}
