#include "diffuse_emitter.h"

radiance::materials::DiffuseEmitter::DiffuseEmitter(std::shared_ptr<Texture> texture):
    Material{texture}
{
}

radiance::materials::DiffuseEmitter::DiffuseEmitter(math::Color3 reflectance):
    Material{reflectance}
{
}

bool radiance::materials::DiffuseEmitter::bounce(const math::Ray &in, const geometry::Hit &hit, math::Color3 &attenutation, math::Ray &out) const
{
    return false;
}

radiance::math::Color3 radiance::materials::DiffuseEmitter::emit(const geometry::Hit &hit) const
{
    return texture->getColor(hit);
}
