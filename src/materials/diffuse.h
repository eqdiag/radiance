#pragma once

#include "material.h"

namespace radiance{

    namespace materials{

        class Diffuse: public Material{
            public:
                Diffuse(std::shared_ptr<Texture> texture);
                Diffuse(math::Color3 reflectance);

                bool bounce(const math::Ray& in,const geometry::Hit& hit,math::Color3& attenutation,math::Ray& out) const override;

        };
    }
}