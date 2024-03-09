#include "rand.h"

#include <cstdlib>
#include <ctime>

void math::initRandom()
{
    srand (static_cast<unsigned> (time(0)));
}

float math::randomFloat(float min, float max)
{
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r*(max - min) + min;
}

math::Vec3 math::randomUnitDisk()
{
    while(true){
        auto v = math::Vec3{randomFloat(-1,1),randomFloat(-1,1)};
        if(v.dot(v) < 1.0f) return v;
    }
}
