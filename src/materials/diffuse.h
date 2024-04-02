#pragma once

#include "material.h"

namespace materials{

    class Diffuse: public Material{
        public:
            Diffuse();
            Diffuse(math::Color3 albedo);
            Diffuse(std::shared_ptr<radiance::textures::Texture> texture);
            bool bounce(
                const math::Vec3& in,
                const radiance::geometry::Hit& hit,
                math::Vec3& attenuation,
                math::Vec3& out
                ) const override;

    };
}