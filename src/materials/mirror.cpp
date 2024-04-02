#include "mirror.h"

#include "math/util.h"

materials::Mirror::Mirror(std::shared_ptr<radiance::textures::Texture> texture):
    Material{texture}
{

}

materials::Mirror::Mirror(math::Color3 albedo): 
    Material{std::make_shared<radiance::textures::ConstantTexture>(albedo)}
{

}

materials::Mirror::Mirror():
    Mirror{math::Color3{1,1,1}}
{

}



bool materials::Mirror::bounce(const math::Vec3 &in, const radiance::geometry::Hit &hit, math::Vec3 &attenuation, math::Vec3 &out) const
{
    auto uv = hit.uv;
    attenuation = _texture->getValue(uv.x(),uv.y(),hit.point);
    out = math::reflect(in,hit.normal);
    return true;
}

