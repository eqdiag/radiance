#pragma once


#include "math/vec.h"

namespace math{

    void initRandom();

    float randomFloat(float min = 0.0f,float max = 1.0f);

    int randInt(int min,int max);

    /* Uses rejection sampling to get a point in unit disk*/
    /*Note: z coordinate is 0*/
    Vec3 randomUnitDisk();
}