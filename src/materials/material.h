#pragma once

#include "math/ray.h"
#include "geometry/hit.h"

namespace radiance{

    namespace materials{

        class Material{
            public:
        
                virtual bool bounce(const math::Ray& in,const geometry::Hit& hit,math::Color3& attenutation,math::Ray out) const = 0;
        };
    }
}