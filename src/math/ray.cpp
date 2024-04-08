#include "ray.h"

radiance::math::Ray::Ray()
{
}

radiance::math::Ray::Ray(Vec3 src, Vec3 dir) : p{src},
                                               v{dir.normalize()}
{
    
}

radiance::math::Vec3 radiance::math::Ray::cast(float t) const
{
    return p + v*t;
}
