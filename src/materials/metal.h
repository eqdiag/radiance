#pragma once

#include "material.h"

namespace radiance{

    namespace materials{

        class Metal: public Material{
            public:
                Metal(math::Color3 reflectance,float blur);

                bool bounce(const math::Ray& in,const geometry::Hit& hit,math::Color3& attenutation,math::Ray& out) const override;

            private:
                float blur{};
        };
    }
}