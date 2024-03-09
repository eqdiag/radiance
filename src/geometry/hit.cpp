#include "hit.h"

void geometry::Hit::setFaceNormal(const math::Vec3 &in, const math::Vec3 &n)
{
    outside = in.dot(n) < 0.0f;
    normal = outside ? n : -n;
}