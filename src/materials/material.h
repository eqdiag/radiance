#pragma once

#include "math/vec.h"
#include "geometry/hit.h"
#include "texture.h"

namespace materials{

    class Material{
        public:
            
            Material(std::shared_ptr<radiance::textures::Texture> texture);

            virtual bool bounce(
                const math::Vec3& in,
                const radiance::geometry::Hit& hit,
                math::Vec3& attenuation,
                math::Vec3& out
                ) const = 0;

            virtual math::Color3 emit(float u,float v,math::Vec3 p) const{
                return math::Color3{0,0,0};
            };

            void setTexture(std::shared_ptr<radiance::textures::Texture> texture);

        protected:
            std::shared_ptr<radiance::textures::Texture> _texture{};
    };
}