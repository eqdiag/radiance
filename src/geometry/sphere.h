#pragma once

#include "math/vec.h"
#include "math/ray.h"
#include "geometry/hit.h"
#include "materials/material.h"

namespace geometry{

    class Sphere: public Hittable{
        public:
            Sphere(math::Vec3& center,float radius);
            Sphere(math::Vec3& center,float radius,std::shared_ptr<materials::Material> material);

            bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;

        private:
            math::Vec3 _center{};
            float _radius{};
    };


}