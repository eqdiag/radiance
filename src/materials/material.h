#pragma once

#include "math/vec.h"
#include "geometry/hit.h"

namespace materials{

    class Material{
        public:
            virtual bool bounce(
                const math::Vec3& in,
                const geometry::Hit& hit,
                math::Vec3& attenuation,
                math::Vec3& out
                ) const = 0;
    };
}