#pragma once

#include "math/ray.h"
#include "geometry/hit.h"
#include "texture.h"

#include <memory>

namespace radiance{

    namespace materials{

        class Material{
            public:
                Material(math::Color3 color);
                Material(std::shared_ptr<Texture> texture);

                virtual bool bounce(const math::Ray& in,const geometry::Hit& hit,math::Color3& attenutation,math::Ray& out) const = 0;
            
            protected:
                std::shared_ptr<Texture> texture{};
        };
    }
}