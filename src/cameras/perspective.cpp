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
    _eye{eye},
    _right{right.normalize()},
    _up{up.normalize()},
    _front{front.normalize()*focalLength},
    _view_u{right*view_width},
    _view_v{-up*view_height},
    _du{_view_u/width},
    _dv{_view_v/height},
    _corner{eye + front - _view_u*0.5 - _view_v*0.5 + _du*0.5 + _dv*0.5}    
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
    auto rdx = _du*(radiance::math::randomFloat() - 0.5)+ _dv*(radiance::math::randomFloat() - 0.5);

    auto film_pt = _corner + _du*j + _dv*i + rdx;
    return radiance::math::Ray{_eye,(film_pt - _eye)};
}
