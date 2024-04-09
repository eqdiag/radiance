#pragma once

#include "geometry/hit.h"
#include "geometry/triangle.h"

#include <vector>

namespace radiance{

    namespace io{

        /* Reads */

        bool readTriangleListFromObj(std::vector<geometry::Vertex>& vertices,std::vector<uint32_t>& indices,const char* filename);

    }
}