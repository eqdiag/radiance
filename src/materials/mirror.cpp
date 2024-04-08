#include "mirror.h"
#include "math/util.h"

radiance::materials::Mirror::Mirror(math::Color3 reflectance):
    Material{reflectance}
{
}

bool radiance::materials::Mirror::bounce(const math::Ray &in, const geometry::Hit &hit, math::Color3 &attenutation, math::Ray &out) const
{

    //Fresnel term
    auto reflectance = texture->getColor(hit);
    float cos = - in.v.dot(hit.n);
    auto f = math::schlick(cos,reflectance);


    attenutation = f;

    auto v_out = math::reflect(in.v,hit.n);
    out = math::Ray{hit.p,v_out};

    return true;
}
