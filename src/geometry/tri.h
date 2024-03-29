#pragma once

#include "geometry/hit.h"
#include "materials/material.h"

namespace radiance{

    namespace geometry{

        class Tri: public Hittable{
            public:
                /* Vertices assumed in CCW order */
                Tri(const math::Vec3& p0,const math::Vec3& p1,const math::Vec3& p2,std::shared_ptr<materials::Material> material);

                void computeBoundingBox();

                bool trace(const math::Ray&,Hit&,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;
                bool getBoundingBox(radiance::geometry::AABB& box) const override;


            private:
                math::Vec3 _p0{};
                math::Vec3 _p1{};
                math::Vec3 _p2{};
                math::Vec3 _p01{};
                math::Vec3 _p12{};
                math::Vec3 _p20{};

                math::Vec3 _normal{};

                std::shared_ptr<materials::Material> _material{};

                geometry::AABB _box{};
        };
    }

}