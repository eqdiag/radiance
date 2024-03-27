#pragma once

#include "geometry/hit.h"
#include "math/vec.h"

#include <vector>

namespace radiance{

    namespace geometry{


        class TriMesh: public Hittable{
            public:
                TriMesh();
                TriMesh(std::shared_ptr<materials::Material> material);
                TriMesh(std::vector<math::Vec3>&& vertices,std::vector<uint32_t>&& indices,std::shared_ptr<materials::Material> material);

                void setVertices(std::vector<math::Vec3>&& vertices);
                void setIndices(std::vector<uint32_t>&& indices);


                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;


                std::shared_ptr<materials::Material> getMaterial();
                void getFaceVertices(uint32_t faceIndex,math::Vec3& v0,math::Vec3& v1,math::Vec3& v2) const;
                uint32_t getNumFaces() const;

            private:
                std::vector<math::Vec3> _vertices{};
                std::vector<uint32_t> _indices{};
                uint32_t _numFaces{};
        };

    }
}