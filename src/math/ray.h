#pragma once

#include "vec.h"

namespace math{

    class Ray{
        public:
            Ray(Vec3 src,Vec3 dir);

            Vec3 cast(float t) const;

            const Vec3& getSrc() const;
            const Vec3& getDir() const;

        private:
            math::Vec3 _src;
            math::Vec3 _dir;
    };

}