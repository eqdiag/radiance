#pragma once

#include <vector>

#include "matrix.h"

namespace radiance{

    namespace math{

        class Transform{
            public:

        
                Transform& scale(Vec3 s);
                Transform& translate(Vec3 t);
                Transform& rotate(float angle,Vec3 axis);
                
                void clear();

                Mat4 matrix{};
                Mat4 inverse{};
                
                friend std::ostream& operator<<(std::ostream& out, const Transform& v);

             
        };

    }

}