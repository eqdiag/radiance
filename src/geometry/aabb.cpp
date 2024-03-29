#include "aabb.h"

radiance::geometry::AABB::AABB(math::Vec3 min, math::Vec3 max)
:min{min},max{max}
{

}

radiance::geometry::AABB radiance::geometry::AABB::merge(const AABB &rhs)
{
    return AABB{
        min.min(rhs.min),
        max.max(rhs.max)
    };
}

math::Vec3 radiance::geometry::AABB::getCentroid() const
{
    return math::Vec3{
        0.5f*(min.x() + max.x()),
        0.5f*(min.y() + max.y()),
        0.5f*(min.z() + max.z())
    };
}

bool radiance::geometry::AABB::isHit(const math::Ray &ray, float& tmin, float& tmax) const
{
    auto origin = ray.getSrc();
    auto dir = ray.getDir();
    //Andrew Kensler's intersection routine
    for (int a = 0; a < 3; a++) {

        auto invD = 1.0f / dir[a];
        auto t0 = (min[a] - origin[a]) * invD;
        auto t1 = (max[a] - origin[a]) * invD;
        if (invD < 0.0f) std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin) return false;
    }

    return true;   
}

