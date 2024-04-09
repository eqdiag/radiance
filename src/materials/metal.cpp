#include "metal.h"
#include "math/util.h"

radiance::materials::Metal::Metal(math::Color3 reflectance,float blur):
    Material{reflectance},
    blur{(blur > 1.0f) ? 1.0f : blur}
{
}

bool radiance::materials::Metal::bounce(const math::Ray &in, const geometry::Hit &hit, math::Color3 &attenutation, math::Ray &out) const
{
    //Fresnel term
    auto reflectance = texture->getColor(hit);
    float cos = - in.v.dot(hit.n);
    auto f = math::schlick(cos,reflectance);


    attenutation = f;

    auto v_out = math::reflect(in.v,hit.n) + math::randomOnUnitSphere()*blur;
    out = math::Ray{hit.p,v_out};

    return true;
}
