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
   

    //Schlick fresnel approximation for reflectance
    auto reflectance = _texture->getValue(uv.x(),uv.y(),hit.point);
    float cos = hit.normal.dot(-in.normalize());
    auto s = (math::Vec3{1,1,1} - reflectance)*pow(1 - cos,5) + reflectance;
    
    attenuation = s;
    out = math::reflect(in,hit.normal);

    return true;
}

