#pragma once

#include "math/vec.h"
#include "math/ray.h"

namespace radiance{

    namespace geometry{

        struct AABB{
            public:
                AABB(){};
                AABB(math::Vec3 min,math::Vec3 max);

                AABB merge(const AABB& rhs);

                math::Vec3 getCentroid() const;

                bool isHit(const math::Ray& ray,float& tmin,float& tmax) const;

                math::Vec3 min{};
                math::Vec3 max{};

        };

    }

}