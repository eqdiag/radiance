#include "diffuse.h"
#include "math/util.h"

#include <iostream>

radiance::materials::Diffuse::Diffuse(std::shared_ptr<Texture> texture):
    Material{texture}
{
    
}

radiance::materials::Diffuse::Diffuse(math::Color3 reflectance) : Material{reflectance}
{
}

bool radiance::materials::Diffuse::bounce(const math::Ray &in, const geometry::Hit &hit, math::Color3 &attenutation, math::Ray &out) const
{

    attenutation = texture->getColor(hit);
    
    auto v = math::randomOnUnitHemisphere(hit.n);
    out = math::Ray{hit.p,v};

    return true;
}