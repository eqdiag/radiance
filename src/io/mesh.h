#pragma once

#include "geometry/tri_mesh.h"

namespace radiance{

    namespace io{

        enum class MeshFileType{
            OBJ,
            PLY
        };

        /* Reads */

        bool readTriMeshFromObj(geometry::TriMesh& mesh,const char* filename);

        bool readTriMeshFromPly(geometry::TriMesh& mesh,const char* filename);

        bool readTriMeshFromFile(geometry::TriMesh&mesh,const char* filename,MeshFileType fileType);

        /* Writes */
    }
}