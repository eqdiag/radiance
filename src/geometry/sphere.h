#pragma once

#include "math/vec.h"
#include "math/ray.h"
#include "geometry/hit.h"
#include "materials/material.h"
#include "geometry/aabb.h"

namespace radiance{

    namespace geometry{

        class Sphere: public Hittable{
            public:
                Sphere(math::Vec3& center,float radius);
                Sphere(math::Vec3& center,float radius,std::shared_ptr<materials::Material> material);

                void computeBoundingBox();

                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;
                bool getBoundingBox(radiance::geometry::AABB& box) const override;



            private:
                math::Vec3 _center{};
                float _radius{};

                radiance::geometry::AABB _box{};

        };


    }
}