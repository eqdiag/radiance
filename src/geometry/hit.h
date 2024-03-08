#pragma once

#include "math/vec.h"

#include "math/ray.h"

//Forward dec
namespace materials{
    class Material;
}

namespace geometry{

    struct Hit{
        math::Vec3 point{};
        math::Vec3 normal{};
        float t;
        std::shared_ptr<materials::Material> _material{};
    };

    class Hittable{
        public:
            virtual bool trace(const math::Ray&,Hit&,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const = 0;
    };
}