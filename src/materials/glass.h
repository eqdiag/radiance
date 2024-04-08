#pragma once

#include "material.h"

namespace radiance{

    namespace materials{

        class Glass: public Material{
            public:

                Glass(float refractionIndex);

                bool bounce(const math::Ray& in,const geometry::Hit& hit,math::Color3& attenutation,math::Ray& out) const override;

            private:
                float refractionIndex;
        };
    }
}