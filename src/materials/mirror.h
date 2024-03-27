#pragma once

#include "material.h"

namespace materials{

    class Mirror: public Material{
        public:
            Mirror();
            Mirror(math::Color3 albedo);
            bool bounce(
                const math::Vec3& in,
                const radiance::geometry::Hit& hit,
                math::Vec3& attenuation,
                math::Vec3& out
                ) const override;

        private:
            math::Color3 _albedo{};
    };
}