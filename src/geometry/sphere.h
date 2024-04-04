#pragma once

#include "math/vec.h"
#include "hit.h"

namespace radiance{

    namespace geometry{

        struct Sphere: public Hittable{

            Sphere(math::Vec3 center,float radius);

            bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;


            math::Vec3 _center;
            float _radius;
        };
    }
}