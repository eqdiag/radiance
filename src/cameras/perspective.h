#pragma once

#include "math/vec.h"
#include "math/ray.h"

namespace radiance{

    namespace cameras{

        class Perspective{
            public:

                Perspective(const Perspective&) = delete;
                Perspective operator=(const Perspective&) = delete;

                Perspective(const Perspective&&) = delete;
                Perspective operator=(const Perspective&&) = delete;

                //Assumes (eye,right,up,front) forms an orthogonal frame
                Perspective(
                    float view_width,
                    float view_height,
                    int width,
                    int height,
                    radiance::math::Vec3 eye = radiance::math::Vec3{0,0,0},
                    radiance::math::Vec3 right = radiance::math::Vec3{1,0,0},
                    radiance::math::Vec3 up = radiance::math::Vec3{0,1,0},
                    radiance::math::Vec3 front = radiance::math::Vec3{0,0,-1},
                    float focalLength = 1.0f
                );

                //Create camera based on look location
                //vfov: vertical fov in degrees
                static Perspective lookAt(
                    float aspect_ratio,
                    float vfov,
                    int width,
                    math::Vec3 from = radiance::math::Vec3{0,0,0},
                    math::Vec3 at = radiance::math::Vec3{0,0,-1},
                    math::Vec3 up = radiance::math::Vec3{0,1,0},
                    float focalLength = 1.0f
                );


                radiance::math::Ray generateRay(int i,int j) const;

    

            private:

                //Frame of camera
                radiance::math::Vec3 eye;
                radiance::math::Vec3 right;
                radiance::math::Vec3 up;
                radiance::math::Vec3 front;


                //Right and left directions from corner
                radiance::math::Vec3 view_u;
                radiance::math::Vec3 view_v;

                //Offsets between pixel centers
                radiance::math::Vec3 dx;
                radiance::math::Vec3 dy;

                //Top-left corner of film (offset by half of dx,dy)
                radiance::math::Vec3 corner;

        };

    }
}