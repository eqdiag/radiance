#pragma once

#include "material.h"

namespace radiance{

    namespace materials{

        class Mirror: public Material{
            public:

                Mirror(math::Color3 reflectance);

                bool bounce(const math::Ray& in,const geometry::Hit& hit,math::Color3& attenutation,math::Ray& out) const override;

            private:
                math::Vec3 reflectance{};
        };
    }
}