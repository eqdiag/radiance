#pragma once

#include <vector>

#include "matrix.h"


namespace math{

    class Transform{
        public:

    
            Transform& scale(math::Vec3& s);
            Transform& translate(math::Vec3 t);
            Transform& rotate(float angle,math::Vec3& axis);
            Transform& matrix(math::Mat4& matrix);

            math::Mat4 getMatrix() const;
            math::Mat4 getInverseMatrix() const;
            
            friend std::ostream& operator<<(std::ostream& out, const Transform& v);


        private:
            math::Mat4 _matrix{};
            math::Mat4 _inverse{};
    };


}