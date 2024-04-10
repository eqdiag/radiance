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
    float f = math::schlick(cos,1.0/1.7);

    

    //Normal reflection
    float r = math::randomFloat();
    if(r < f){
        attenutation = math::Vec3{1,1,1};
        auto v_out = math::reflect(in.v,hit.n);
        out = math::Ray{hit.p,v_out};
        return true;
    }else{ //Refract into diffuse material
        auto v_out = math::randomOnUnitHemisphere(hit.n);
        out = math::Ray{hit.p,v_out};
        attenutation = texture->getColor(hit);
        return true;
    }
}
