#pragma once

#include "hit.h"
#include "math/vec.h"
#include "aabb.h"

#include "materials/material.h"

namespace radiance{

    namespace geometry{

        struct Vertex{
            math::Vec3 p;
            math::Vec3 n;
            math::Vec2 uv;
        };

        class Triangle: public Hittable{
            public:
                Triangle(Vertex& p0,Vertex& p1,Vertex& p2,std::shared_ptr<materials::Material> material);

                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;

                //Get bounding box if the object has one
                bool boundingBox(geometry::AABB& outBox) const override;

                std::shared_ptr<materials::Material> material{};
                geometry::AABB box{};

            private:
                Vertex v0{};
                Vertex v1{};
                Vertex v2{};
                math::Vec3 v01,v12,v20;
                math::Vec3 faceNormal;
                float area;

        };
    }
}