#pragma once

#include "math/vec.h"

#include "math/ray.h"
#include "math/transform.h"


//Forward dec
namespace materials{
    class Material;
}


namespace radiance{

    namespace geometry{

        struct Hit{
            math::Vec3 point{};
            math::Vec3 normal{};
            //Hit from outside, or inside
            bool outside{};
            float t;
            std::shared_ptr<materials::Material> _material{};

            void setFaceNormal(const math::Vec3& in,const math::Vec3& n);

        };

        
        class Hittable{
            public:
                virtual bool trace(const math::Ray&,Hit&,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const = 0;

                void setMaterial(std::shared_ptr<materials::Material> material);

                std::shared_ptr<materials::Material> _material{};

            protected:
        };

        class InstancedHittable: public Hittable{
            public:
                InstancedHittable(std::shared_ptr<Hittable> object,math::Transform transform);

                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const;

            private:
                std::shared_ptr<Hittable> _object;
                math::Transform _transform;
        };
    }
}