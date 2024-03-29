#pragma once

#include "math/vec.h"

#include "math/ray.h"
#include "math/transform.h"
#include "geometry/aabb.h"

#include <optional>

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
                virtual bool getBoundingBox(radiance::geometry::AABB& box) const = 0;
                //Some hittable objects may contain many `subprimitives` (e.g. triangle mesh, others might not e.g. sphere)
                virtual std::optional<std::vector<std::shared_ptr<geometry::Hittable>>> getPrimitives(const math::Transform& transform = math::Transform{});


                virtual void setMaterial(std::shared_ptr<materials::Material> material);

                std::shared_ptr<materials::Material> _material{};

            protected:
        };

        class InstancedHittable: public Hittable{
            public:
                InstancedHittable(std::shared_ptr<Hittable> object,math::Transform transform);

                void computeBoundingBox();

                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;
                bool getBoundingBox(radiance::geometry::AABB& box) const override;
                std::optional<std::vector<std::shared_ptr<geometry::Hittable>>> getPrimitives(const math::Transform& transform = math::Transform{}) override;
                void setMaterial(std::shared_ptr<materials::Material> material) override;


            private:
                std::shared_ptr<Hittable> _object;
                math::Transform _transform;

                geometry::AABB _box{};
        };
    }
}