#include "plastic.h"

#include "math/util.h"

radiance::materials::Plastic::Plastic(std::shared_ptr<Texture> texture):
    Material{texture}
{
}

radiance::materials::Plastic::Plastic(math::Color3 reflectance) : Material{reflectance}
{
}

bool radiance::materials::Plastic::bounce(const math::Ray &in, const geometry::Hit &hit, math::Color3 &attenutation, math::Ray &out) const
{

    float cos = - in.v.dot(hit.n);
    float f = math::schlick(cos,1.0/1.5);

    attenutation = texture->getColor(hit);

    //Normal reflection
    if(math::randomFloat() < f){
        auto v_out = math::reflect(in.v,hit.n);
        out = math::Ray{hit.p,v_out};
    }else{ //Refract into diffuse material
        auto v = math::randomOnUnitHemisphere(hit.n);
        out = math::Ray{hit.p,v};
    }

    return true;
}
