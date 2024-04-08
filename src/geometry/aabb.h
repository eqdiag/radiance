#pragma once

#include "hit.h"

namespace radiance{

    namespace geometry{

        class AABB{
            public:
                AABB(){}
                AABB(math::Vec3 min,math::Vec3 max);

                AABB merge(const AABB& rhs);

                bool hit(const math::Ray& ray,float& tmin,float& tmax) const;


                math::Vec3 min{};
                math::Vec3 max{};
        };
    }
}