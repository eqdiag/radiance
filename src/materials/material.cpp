#include "material.h"

radiance::materials::Material::Material(math::Color3 color):
    texture{std::make_shared<ConstantTexture>(color)}
{
}

radiance::materials::Material::Material(std::shared_ptr<Texture> texture): 
    texture{texture}
{

}

radiance::math::Color3 radiance::materials::Material::emit(const geometry::Hit &hit) const
{
    return math::BLACK;
}
