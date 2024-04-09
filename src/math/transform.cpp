#include "transform.h"

radiance::math::Transform &radiance::math::Transform::scale(Vec3 s)
{
    matrix = Mat4::fromScale(s.x(),s.y(),s.z()) * matrix;
    inverse = inverse * Mat4::fromScale(1.0/s.x(),1.0/s.y(),1.0/s.z());
    return *this;
}

radiance::math::Transform &radiance::math::Transform::translate(Vec3 t)
{
    matrix = Mat4::fromTranslation(t.x(),t.y(),t.z()) * matrix;
    inverse = inverse * Mat4::fromTranslation(-t.x(),-t.y(),-t.z());
    return *this;
}

radiance::math::Transform &radiance::math::Transform::rotate(float angle, Vec3 axis)
{
    matrix = Mat4::fromAxisAngle(angle,axis) * matrix;
    inverse = inverse * Mat4::fromAxisAngle(-angle,axis);
    return *this;
}

std::ostream &radiance::math::operator<<(std::ostream &out, const Transform &v)
{
    out << v.matrix;
    return out;
}
