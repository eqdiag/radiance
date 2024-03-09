#include "perspective_camera.h"

#include "math/util.h"
#include "math/rand.h"

cameras::PerspectiveCamera::PerspectiveCamera(
    const ViewingPlane &plane,
    float vfov,
    float lensRadius,
    math::Vec3 lookFrom,
    math::Vec3 lookAt,
    math::Vec3 up
):
    _viewHeight{static_cast<float>(2.0 * plane.focalLength * tan(math::degToRad(vfov)*0.5))},
    _viewWidth{ _viewHeight * plane.aspectRatio},
    _lensRadius{lensRadius},
    _eye{lookFrom},
    _front{(lookAt - lookFrom).normalize()},
    _up{up.normalize()},
    _right{_front.cross(_up)},
    _dx{_right * _viewWidth / plane.imageWidth},
    _dy{ - _up * _viewHeight / plane.imageHeight},
    _topLeft{_eye + _front*plane.focalLength - _right*_viewWidth*0.5 + _up*_viewHeight*0.5},
    _topLeftPixelCenter{_topLeft + _dx*0.5 + _dy*0.5}
{

}

math::Ray cameras::PerspectiveCamera::generateRay(float x, float y) const
{
    auto view_pt = _topLeftPixelCenter + _dx*x + _dy*y;
    auto dr = math::randomUnitDisk()*_lensRadius;
    auto eye_pt = _eye + _right*dr.x() + _up*dr.y();
    return math::Ray{eye_pt,view_pt - eye_pt};
}
