#pragma once

#include "geometry/hit.h"
#include "math/vec.h"

#include "geometry/aabb.h"
#include "acceleration/bvh.h"

#include <vector>

namespace radiance{

    namespace geometry{


        class TriMesh: public Hittable{
            public:
                TriMesh();
                TriMesh(std::shared_ptr<materials::Material> material);
                TriMesh(std::vector<Vertex>&& vertices,std::vector<uint32_t>&& indices);
                TriMesh(std::vector<Vertex>&& vertices,std::vector<uint32_t>&& indices,std::shared_ptr<materials::Material> material);

                void setVertices(std::vector<Vertex>&& vertices);
                void setIndices(std::vector<uint32_t>&& indices);


                void computeBoundingBox();

                std::shared_ptr<acceleration::BVHNode> generateBVH();

                void enableVertexNormals();

                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;
                bool getBoundingBox(radiance::geometry::AABB& box) const override;
                std::optional<std::vector<std::shared_ptr<geometry::Hittable>>> getPrimitives(const math::Transform& transform = math::Transform{}) override;


                std::shared_ptr<materials::Material> getMaterial();
                void getFaceVertices(uint32_t faceIndex,Vertex& v0,Vertex& v1,Vertex& v2) const;
                uint32_t getNumFaces() const;

            private:
                std::vector<Vertex> _vertices{};
                std::vector<uint32_t> _indices{};
                uint32_t _numFaces{};

                radiance::geometry::AABB _box{};

                bool _vertexNormals = false;
        };

    }
}