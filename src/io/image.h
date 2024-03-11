#pragma once

#include "geometry/tri_mesh.h"

namespace radiance{

    namespace io{

        /* Reads */

        /* Writes */
        bool writeBufferToRGB_PNG(const char* buffer,int width,int height,const char* filename);

    }
}