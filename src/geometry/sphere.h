#pragma once

#include "hit.h"
#include "geometry/aabb.h"
#include "math/vec.h"
#include "materials/material.h"

namespace radiance{

    namespace geometry{

        struct Sphere: public Hittable{

            Sphere(math::Vec3 center,float radius,std::shared_ptr<materials::Material> material);

            bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;

            bool boundingBox(geometry::AABB& outBox) const override;

            std::shared_ptr<materials::Material> getMaterial() const override;


            math::Vec3 _center;
            float _radius;
            geometry::AABB box{};
            std::shared_ptr<materials::Material> material{};

            private:
                math::Vec2 pointToUV(const math::Vec3& p) const;

        };
    }
}