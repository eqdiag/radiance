#include "plastic.h"
#include "math/util.h"
#include "math/rand.h"

materials::Plastic::Plastic(float index):
    Material{std::make_shared<radiance::textures::ConstantTexture>(math::Color3{1,1,1})},
    _index{index},
    _glass{std::make_shared<Glass>(index)},
    _diffuse{std::make_shared<Diffuse>(_texture)}
{

}

materials::Plastic::Plastic(float index, math::Color3 albedo):
    Material{std::make_shared<radiance::textures::ConstantTexture>(albedo)},
    _index{index},
    _glass{std::make_shared<Glass>(index)},
    _diffuse{std::make_shared<Diffuse>(_texture)}
{

}

materials::Plastic::Plastic(float index, std::shared_ptr<radiance::textures::Texture> texture):
    Material{texture},
    _index{index},
    _glass{std::make_shared<Glass>(index)},
    _diffuse{std::make_shared<Diffuse>(_texture)}
{

}

bool materials::Plastic::bounce(const math::Vec3 &in, const radiance::geometry::Hit &hit, math::Vec3 &attenuation, math::Vec3 &out) const
{

    float cos = hit.normal.dot(-in.normalize());

    float f = math::schlick(cos,1.0/_index);

    //For now, diffuse just holds a constant tex informed color
    auto diffuse = _texture->getValue(hit.uv.x(),hit.uv.y(),hit.point);


    float r = math::randomFloat();
    if(r < f){ //Reflect
        out = math::reflect(in,hit.normal);
        attenuation = math::Vec3{1,1,1};
        return true;
    }else{ //Just diffuse
        attenuation = _texture->getValue(hit.uv.x(),hit.uv.y(),hit.point);
        return false;
    }

}
