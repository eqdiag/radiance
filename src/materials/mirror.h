#pragma once

#include "material.h"

namespace materials{

    class Mirror: public Material{
        public:
            Mirror(std::shared_ptr<radiance::textures::Texture> texture);
            Mirror(math::Color3 albedo);
            Mirror();
            bool bounce(
                const math::Vec3& in,
                const radiance::geometry::Hit& hit,
                math::Vec3& attenuation,
                math::Vec3& out
                ) const override;

    };
}