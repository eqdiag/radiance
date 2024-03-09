#pragma once

#include "vec.h"

namespace math{

    constexpr float PI = 3.141592653589793;
    constexpr float TWO_PI = 6.28318530718;

    float degToRad(float deg);
    float radToDeg(float rad);

    /* Assumes in, and normal are in different directions: in.dot(normal) < 0 */
    /* Also assumes both vectors are normalized */
    Vec3 reflect(const Vec3& in,const Vec3& normal);

    /* Assumes in, and normal are in different directions: in.dot(normal) < 0 */
    /* Also assumes both vectors are normalized */
    Vec3 refract(const Vec3& in,const Vec3& normal,float indexRatio);

    /* Schlick approximation for total internal reflection*/
    float schlick(float cos,float indexRatio);

    

}