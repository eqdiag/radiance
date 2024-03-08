#pragma once

#include "math/vec.h"
#include "math/ray.h"

namespace cameras{

    struct ViewingPlane{
        float aspectRatio;
        float focalLength;
        int imageWidth;
        int imageHeight;
        int spp;
    };

    class PerspectiveCamera{
        public:
            /* User responsibility to ensure frame is orthonormal */
            PerspectiveCamera(
                const ViewingPlane& plane,
                float vfov = 90.0f,
                math::Vec3 lookFrom = math::Vec3{0,0,0},
                math::Vec3 lookAt = math::Vec3{0,0,-1},
                math::Vec3 up = math::Vec3{0,1,0}
            );

            /* Coordinate system where (0,0) is top-left, (w,h) is bottom-right */
            math::Ray generateRay(float x,float y) const;

        private:

            float _viewHeight{};
            float _viewWidth{};

            math::Vec3 _eye{};

            math::Vec3 _front{};
            math::Vec3 _up{};
            math::Vec3 _right{};

            math::Vec3 _dx{};
            math::Vec3 _dy{};

            math::Vec3 _topLeft{};
            math::Vec3 _topLeftPixelCenter{};
    };


}