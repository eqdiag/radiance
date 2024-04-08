#include "util.h"

float radiance::math::degToRad(float deg)
{
    return (deg / 180.0f) * PI;
}

float radiance::math::radToDeg(float rad)
{
    return (rad * 180.f) / PI;
}

radiance::math::Vec3 radiance::math::reflect(const Vec3 &in, const Vec3 &normal)
{
    return in - normal*2.0f*in.dot(normal);
}

radiance::math::Vec3 radiance::math::refract(const Vec3 &in, const Vec3 &normal, float indexRatio)
{
    float cos = - in.dot(normal);
    auto in_perp = (in + normal * cos) * indexRatio;
    auto in_n = normal * -sqrt(1.0f - in_perp.dot(in_perp));
    return in_perp + in_n;
}

float radiance::math::schlick(float cos, float indexRatio)
{
    float f0 = (1.0f - indexRatio) / (1.0 + indexRatio);
    f0 = f0*f0;
    return f0 + (1.0 - f0)*pow(1.0 - cos,5);
}

radiance::math::Vec3 radiance::math::schlick(float cos, Vec3 R0)
{
    return R0 + (math::Vec3{1,1,1} - R0)*pow(1.0 - cos,5);
}

void radiance::math::initRandom()
{
    srand (static_cast<unsigned> (time(0)));
}

//Return random float in [min,max)
float radiance::math::randomFloat(float min, float max)
{
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r*(max - min) + min;
}


//Return random int in [min,max]
int radiance::math::randInt(int min, int max)
{
    return static_cast<int>(randomFloat(min,max + 1));
}

radiance::math::Vec3 radiance::math::randomUnitDisk()
{
    while(true){
        auto v = math::Vec3{randomFloat(-1,1),randomFloat(-1,1)};
        if(v.dot(v) < 1.0f) return v;
    }
}

radiance::math::Vec3 radiance::math::randomUnitSphere()
{
    while(true){
        auto v = math::Vec3{randomFloat(-1,1),randomFloat(-1,1),randomFloat(-1,1)};
        if(v.norm2() <= 1.0f) return v;
    };
}

radiance::math::Vec3 radiance::math::randomOnUnitSphere()
{
    return randomUnitSphere().normalize();
}

radiance::math::Vec3 radiance::math::randomOnUnitHemisphere(const math::Vec3 &n)
{
    while(true){
        auto v = randomOnUnitSphere();
        if(v.dot(n) >= 0.0 ) return v;
    };
}

float radiance::math::linearToGamma(float value, float gamma)
{
    return sqrt(value);
}

radiance::math::Color3 radiance::math::linearToGamma(Color3 value, float gamma)
{
    return math::Color3{linearToGamma(value.x(),gamma),linearToGamma(value.y(),gamma),linearToGamma(value.z(),gamma)};
}

void radiance::math::Timer::setName(const char *name)
{
    name = name;
}

void radiance::math::Timer::begin()
{
    start = std::chrono::high_resolution_clock::now();
}

void radiance::math::Timer::stop()
{
    end = std::chrono::high_resolution_clock::now();
}

void radiance::math::Timer::displaySeconds() const
{
    float value = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000000.0f;
    if(value > 60.0f){
        std::cout << name << " took: " << value / 60.0f << " minutes, " << static_cast<float>(fmod(value,60.0f)) << " seconds\n";
    }else{
        std::cout << name << " took: " << value << " seconds\n";
    }

}