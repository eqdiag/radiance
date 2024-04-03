#include "diffuse.h"

#include <cassert>

materials::Diffuse::Diffuse():
    Material{std::make_shared<radiance::textures::ConstantTexture>(math::Color3{1,1,1})}
{
}

materials::Diffuse::Diffuse(math::Color3 albedo) : Material{std::make_shared<radiance::textures::ConstantTexture>(albedo)}
{
}

materials::Diffuse::Diffuse(std::shared_ptr<radiance::textures::Texture> texture):
Material{texture}
{
}

bool materials::Diffuse::bounce(const math::Vec3 &in, const radiance::geometry::Hit &hit, math::Vec3 &attenuation, math::Vec3 &out) const
{
    //Change later
    //TODO: Include u,v coords from hit
    auto uv = hit.uv;
    attenuation = _texture->getValue(uv.x(),uv.y(),hit.point);
    return false;
}

