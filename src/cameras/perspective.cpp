#include "perspective.h"
#include "math/util.h"

#include <iostream>

radiance::cameras::Perspective::Perspective
(
    float view_width,
    float view_height,
    int width,
    int height,
    radiance::math::Vec3 eye,
    radiance::math::Vec3 right,
    radiance::math::Vec3 up,
    radiance::math::Vec3 front,
    float focalLength
):
    eye{eye},
    right{right.normalize()},
    up{up.normalize()},
    front{front.normalize()*focalLength},
    view_u{right*view_width},
    view_v{-up*view_height},
    dx{view_u/width},
    dy{view_v/height},
    corner{eye + front - view_u*0.5 - view_v*0.5}    
{
    

}

radiance::cameras::Perspective radiance::cameras::Perspective::lookAt(
    float aspect_ratio,
    float vfov,
    int width,
    math::Vec3 from,
    math::Vec3 at,
    math::Vec3 up,
    float focalLength)
{
    return Perspective{
        2.0f * focalLength * static_cast<float>(tan(math::degToRad(vfov)*0.5)),
        2.0f * focalLength * static_cast<float>(tan(math::degToRad(vfov)*0.5)) / aspect_ratio,
        width,
        static_cast<int>(width / aspect_ratio),
        from,
        (at - from).cross(up).normalize(),
        (at - from).cross(up).cross(at - from).normalize(),
        (at - from).normalize(),
        focalLength
    };
}

radiance::math::Ray radiance::cameras::Perspective::generateRay(int i, int j) const
{
    //Randomly sample in pixel
    auto rdx = dx*radiance::math::randomFloat(-0.5,0.5) + dy*radiance::math::randomFloat(-0.5,0.5);

    auto film_pt = corner + dx*j + dy*i + rdx;
    return radiance::math::Ray{eye,(film_pt - eye)};
}
