#pragma once

#include "math/vec.h"
#include "math/ray.h"

#include <vector>
#include <memory>
#include <optional>

namespace radiance{

    //Forward declarations
    namespace materials{class Material;}

    namespace geometry{struct AABB;}


    namespace geometry{

        struct Hit{
            float t;
            math::Vec3 p;
            math::Vec3 n;

            //Outside or inside intersection
            bool outside;

            //Texture coordinates
            float u,v;

            std::shared_ptr<materials::Material> material;

            void setNormal(const radiance::math::Ray& ray,const radiance::math::Vec3 normal);
        };

        class Hittable{
            public:
                //Determine if ray hits an object
                //Should nly return true if t in [tmin,tmax]
                virtual bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const = 0;

                //Get bounding box if the object has one
                virtual bool boundingBox(geometry::AABB& outBox) const = 0;
        };


        class HitList: public Hittable{
            public:
                HitList();
                HitList(HitList&& rhs);

                void addObject(std::shared_ptr<Hittable> object);

                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;
                bool boundingBox(geometry::AABB& box) const override;

                std::vector<std::shared_ptr<Hittable>> objects;
        };

    }
}