#include "transform.h"

math::Transform &math::Transform::scale(math::Vec3 &s)
{
    _matrix = math::Mat4::fromScale(s.x(),s.y(),s.z()) * _matrix;
    _inverse = _inverse * math::Mat4::fromScale(1.0/s.x(),1.0/s.y(),1.0/s.z());
    return *this;
}

math::Transform &math::Transform::translate(math::Vec3 t)
{
    _matrix = math::Mat4::fromTranslation(t.x(),t.y(),t.z()) * _matrix;
    _inverse = _inverse * math::Mat4::fromTranslation(-t.x(),-t.y(),-t.z());
    return *this;
}

math::Transform &math::Transform::rotate(float angle, math::Vec3 &axis)
{
    _matrix = math::Mat4::fromAxisAngle(angle,axis) * _matrix;
    _inverse = _inverse * math::Mat4::fromAxisAngle(-angle,axis);
    return *this;
}

math::Transform &math::Transform::matrix(math::Mat4 &matrix)
{
    _matrix = matrix * _matrix;
    std::cerr << "SOFT ERR: General inverse not implemented for matrix!\n";
    return *this;
}

math::Mat4 math::Transform::getMatrix() const
{
    return _matrix;
}

math::Mat4 math::Transform::getInverseMatrix() const
{
    return _inverse;
}

std::ostream &math::operator<<(std::ostream &out, const Transform &v)
{
    out << v._matrix;
    return out;
}
