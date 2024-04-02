#pragma once

#include "geometry/hit.h"
#include "materials/material.h"

namespace radiance{

    namespace geometry{


        class Tri: public Hittable{
            public:
                /* Vertices assumed in CCW order */
                Tri(const Vertex& p0,const Vertex& p1,const Vertex& p2,std::shared_ptr<materials::Material> material,bool vertexNormals = false);

                float getArea() const;

                void computeBoundingBox();

                bool trace(const math::Ray&,Hit&,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;
                bool getBoundingBox(radiance::geometry::AABB& box) const override;


            private:
                Vertex _p0{};
                Vertex _p1{};
                Vertex _p2{};

                math::Vec3 _p01{};
                math::Vec3 _p12{};
                math::Vec3 _p20{};

                bool _vertexNormals = false;

        

                float _area;
                math::Vec3 _normal{};

                std::shared_ptr<materials::Material> _material{};

                geometry::AABB _box{};
        };
    }

}