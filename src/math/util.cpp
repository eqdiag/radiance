#include "util.h"

float math::degToRad(float deg)
{
    return (deg / 180.0f) * PI;
}

float math::radToDeg(float rad)
{
    return (rad * 180.f) / PI;
}

math::Vec3 math::reflect(const Vec3 &in, const Vec3 &normal)
{
    return in - normal*2.0f*in.dot(normal);
}
