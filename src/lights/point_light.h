#pragma once

#include "math/vec.h"

namespace lights{

    struct PointLight{
        math::Vec3 position;
        math::Vec3 intensity;
    };
}