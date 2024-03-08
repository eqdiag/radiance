#include "glass.h"

materials::Glass::Glass(float indexOfRefraction):
    _index{indexOfRefraction}
{
}

bool materials::Glass::bounce(const math::Vec3 &in, const geometry::Hit &hit, math::Vec3 &attenuation, math::Vec3 &out) const
{
    //TODO: 
    return false;
}
