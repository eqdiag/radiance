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

math::Vec3 math::refract(const Vec3 &in, const Vec3 &normal, float indexRatio)
{
    float cos = - in.dot(normal);
    auto in_perp = (in + normal * cos) * indexRatio;
    auto in_n = normal * -sqrt(1.0f - in_perp.dot(in_perp));
    return in_perp + in_n;
}

float math::schlick(float cos, float indexRatio)
{
    float f0 = (1.0f - indexRatio) / (1.0 + indexRatio);
    f0 = f0*f0;
    return f0 + (1.0 - f0)*pow(1.0 - cos,5);
}
