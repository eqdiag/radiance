#pragma once

#include "material.h"
#include "diffuse.h"
#include "glass.h"

namespace materials{

    class Plastic: public Material{
        public:
            Plastic(float index);
            Plastic(float index,math::Color3 albedo);
            Plastic(float index,std::shared_ptr<radiance::textures::Texture> texture);
            bool bounce(
                const math::Vec3& in,
                const radiance::geometry::Hit& hit,
                math::Vec3& attenuation,
                math::Vec3& out
                ) const override;

        private:
            float _index{};
            std::shared_ptr<Glass> _glass{};
            std::shared_ptr<Diffuse> _diffuse{};
    };
}