#pragma once

#include "material.h"

namespace materials{

    class Mirror: public Material{
        public:
            Mirror();
            bool bounce(
                const math::Vec3& in,
                const geometry::Hit& hit,
                math::Vec3& attenuation,
                math::Vec3& out
                ) const override;

        private:
            float _albedo;
    };
}