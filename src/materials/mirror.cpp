#include "mirror.h"

#include "math/util.h"

materials::Mirror::Mirror()
{
}

bool materials::Mirror::bounce(const math::Vec3 &in, const geometry::Hit &hit, math::Vec3 &attenuation, math::Vec3 &out) const
{
    attenuation = math::Vec3{1,1,1};
    out = math::reflect(in,hit.normal);
    return true;
}

