#include "ray.h"

math::Ray::Ray(Vec3 src, Vec3 dir):
    _src{src},
    _dir{dir.normalize()}
{
}

math::Vec3 math::Ray::cast(float t) const
{
    return _src + _dir*t;
}

const math::Vec3 &math::Ray::getSrc() const
{
    return _src;
}

const math::Vec3 &math::Ray::getDir() const
{
    return _dir;
}
