#pragma once

#include "math/vec.h"
#include "math/ray.h"

#include <vector>

namespace radiance{

    namespace geometry{

        struct Hit{
            float t;
            math::Vec3 p;
            math::Vec3 n;
            bool outside;

            void setNormal(const radiance::math::Ray& ray,const radiance::math::Vec3 normal);
        };

        class Hittable{
            public:

                //Determine if ray hits an object
                //Should nly return true if t in [tmin,tmax]
                virtual bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const = 0;
        };


        class HitList: Hittable{
            public:
                HitList();

                void addObject(std::shared_ptr<Hittable> object);

                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;

            private:
                std::vector<std::shared_ptr<Hittable>> objects;
        };

    }
}