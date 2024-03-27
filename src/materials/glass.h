#pragma once

#include "materials/material.h"

namespace materials{

    class Glass: public Material{
    public:
        Glass(float indexOfRefraction);
        bool bounce(
        const math::Vec3& in,
        const radiance::geometry::Hit& hit,
        math::Vec3& attenuation,
        math::Vec3& out
        ) const override;
    private:
        //Index of refraction > 1.0
        float _index{};
    };
}