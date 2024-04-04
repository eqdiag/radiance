#pragma once

#include "vec.h"

namespace radiance{

    namespace math{

        struct Ray{
            public:
                Ray(Vec3 src,Vec3 dir);
                Vec3 cast(float t) const;

                //Ray source
                Vec3 p;
                //Ray direction, normalized
                Vec3 v;
          
        };

    }

}