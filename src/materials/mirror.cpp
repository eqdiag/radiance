#include "mirror.h"

#include "math/util.h"

materials::Mirror::Mirror():
_albedo{math::Color3{1,1,1}}
{
}

materials::Mirror::Mirror(math::Color3 albedo) : _albedo{albedo}
{
}

bool materials::Mirror::bounce(const math::Vec3 &in, const radiance::geometry::Hit &hit, math::Vec3 &attenuation, math::Vec3 &out) const
{
    attenuation = _albedo;
    out = math::reflect(in,hit.normal);
    return true;
}

