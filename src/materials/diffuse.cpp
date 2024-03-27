#include "diffuse.h"


materials::Diffuse::Diffuse(math::Color3 albedo):
    _albedo{albedo}
{
}

bool materials::Diffuse::bounce(const math::Vec3 &in, const radiance::geometry::Hit &hit, math::Vec3 &attenuation, math::Vec3 &out) const
{
    //Change later
    attenuation = _albedo;
    return false;
}

