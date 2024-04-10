#pragma once

#include "material.h"

namespace radiance{

    namespace materials{

        class DiffuseEmitter: public Material{
            public:
                DiffuseEmitter(std::shared_ptr<Texture> texture);
                DiffuseEmitter(math::Color3 reflectance);

                bool bounce(const math::Ray& in,const geometry::Hit& hit,math::Color3& attenutation,math::Ray& out) const override;

                math::Color3 emit(const geometry::Hit& hit) const override;

        };
    }
}