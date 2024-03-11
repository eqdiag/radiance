#include "tri_mesh.h"
#include "tri.h"

#include <optional>

geometry::TriMesh::TriMesh(std::shared_ptr<materials::Material> material):
    _material{material}
{
}

geometry::TriMesh::TriMesh(std::vector<math::Vec3> &&vertices, std::vector<uint32_t> &&indices,std::shared_ptr<materials::Material> material):
    _vertices{vertices},
    _indices{indices},
    _numFaces{static_cast<uint32_t>(_indices.size()/3)},
    _material{material}
{

}

void geometry::TriMesh::setVertices(std::vector<math::Vec3> &&vertices)
{
    _vertices.clear();
    _vertices = vertices;

}

void geometry::TriMesh::setIndices(std::vector<uint32_t> &&indices)
{
    _indices.clear();
    _indices = indices;
    _numFaces = _indices.size() / 3;

    for(int i = 0;i < _numFaces;i++){
        //std::cout << _indices[3*i] << "," << _indices[3*i + 1] << "," << _indices[3*i + 2] << std::endl;
    }
}

bool geometry::TriMesh::trace(const math::Ray &ray, Hit &hit, float tmin, float tmax) const
{
    std::optional<geometry::Hit> closest{};

    for(uint32_t f = 0;f < _numFaces;f++){

        uint32_t idx0 = 3*f;
        uint32_t idx1 = idx0 + 1;
        uint32_t idx2 = idx0 + 2; 

        auto v0 = _vertices[idx0];
        auto v1 = _vertices[idx1];
        auto v2 = _vertices[idx2];


        geometry::Tri tri{v0,v1,v2,_material};
        
        geometry::Hit current{};
        if(tri.trace(ray,current,tmin,tmax)){
            if(closest.has_value()){
                //If there was already a close object, check if something else is closer
                if(current.t < closest.value().t){
                    closest = current;
                }
                //Otherwise, do nothing
            }else{
                //If no objects seen so far, set current to closest
                closest = current;
            }
        }
    }

    bool hit_found = closest.has_value();
    if(hit_found){
        hit = closest.value();
    }

    return hit_found;
}
