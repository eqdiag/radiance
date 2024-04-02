#include "tri_mesh.h"
#include "tri.h"

#include <optional>
#include <cassert>

radiance::geometry::TriMesh::TriMesh()
{
}

radiance::geometry::TriMesh::TriMesh(std::shared_ptr<materials::Material> material)
{
    _material = material;
}

radiance::geometry::TriMesh::TriMesh(std::vector<Vertex> &&vertices, std::vector<uint32_t> &&indices):
_vertices{vertices},
_indices{indices},
_numFaces{static_cast<uint32_t>(_indices.size()/3)}
{

}

radiance::geometry::TriMesh::TriMesh(std::vector<Vertex> &&vertices, std::vector<uint32_t> &&indices,std::shared_ptr<materials::Material> material):
    _vertices{vertices},
    _indices{indices},
    _numFaces{static_cast<uint32_t>(_indices.size()/3)}
{
    _material = material;

}


void radiance::geometry::TriMesh::setVertices(std::vector<Vertex> &&vertices)
{
    _vertices.clear();
    _vertices = vertices;


}

void radiance::geometry::TriMesh::setIndices(std::vector<uint32_t> &&indices)
{
    _indices.clear();
    _indices = indices;
    _numFaces = _indices.size() / 3;
}


void radiance::geometry::TriMesh::computeBoundingBox()
{
     for(uint32_t f = 0;f < _numFaces;f++){

        uint32_t idx0 = 3*f;
        uint32_t idx1 = idx0 + 1;
        uint32_t idx2 = idx0 + 2; 

        auto v0 = _vertices[idx0];
        auto v1 = _vertices[idx1];
        auto v2 = _vertices[idx2];

        geometry::Tri tri{v0,v1,v2,nullptr};
        radiance::geometry::AABB tri_box{};
        if(tri.getBoundingBox(tri_box)){
            _box = _box.merge(tri_box);
        }
     }
}

std::shared_ptr<radiance::acceleration::BVHNode> radiance::geometry::TriMesh::generateBVH()
{
    std::vector<std::shared_ptr<geometry::Hittable>> triangles{};

    for(uint32_t f = 0; f < _numFaces;f++){
        Vertex v0,v1,v2;
        getFaceVertices(f,v0,v1,v2);
        auto tri = std::make_shared<geometry::Tri>(v0,v1,v2,_material,_vertexNormals);
        triangles.emplace_back(tri);
    }

    return std::make_shared<radiance::acceleration::BVHNode>(triangles);
}

void radiance::geometry::TriMesh::enableVertexNormals()
{
    _vertexNormals = true;
}

bool radiance::geometry::TriMesh::trace(const math::Ray &ray, Hit &hit, float tmin, float tmax) const
{
    std::optional<geometry::Hit> closest{};

    for(uint32_t f = 0;f < _numFaces;f++){

        uint32_t idx0 = 3*f;
        uint32_t idx1 = idx0 + 1;
        uint32_t idx2 = idx0 + 2; 

        auto v0 = _vertices[_indices[idx0]];
        auto v1 = _vertices[_indices[idx1]];
        auto v2 = _vertices[_indices[idx2]];


        geometry::Tri tri{v0,v1,v2,nullptr,_vertexNormals};
        
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
        hit._material = _material;
        assert(hit._material != nullptr);
    }
    return hit_found;
}

bool radiance::geometry::TriMesh::getBoundingBox(radiance::geometry::AABB &box) const
{
    box = _box;
    return true;
}

std::optional<std::vector<std::shared_ptr<radiance::geometry::Hittable>>> radiance::geometry::TriMesh::getPrimitives(const math::Transform& transform)
{
    std::vector<std::shared_ptr<geometry::Hittable>> triangles{};
    auto matrix = transform.getMatrix();
    auto inverse = transform.getInverseMatrix();
    auto inverse_transpose = inverse.transpose();

    for(int f = 0;f < _numFaces;f++){
        Vertex v0,v1,v2;
        getFaceVertices(f,v0,v1,v2);
        v0.position = (matrix * math::Vec4{v0.position.x(),v0.position.y(),v0.position.z(),1.0}).xyz();
        v1.position = (matrix * math::Vec4{v1.position.x(),v1.position.y(),v1.position.z(),1.0}).xyz();
        v2.position = (matrix * math::Vec4{v2.position.x(),v2.position.y(),v2.position.z(),1.0}).xyz();


        v0.normal = (inverse_transpose * math::Vec4{v0.normal.x(),v0.normal.y(),v0.normal.z(),0.0}).xyz().normalize();
        v1.normal = (inverse_transpose * math::Vec4{v1.normal.x(),v1.normal.y(),v1.normal.z(),0.0}).xyz().normalize();
        v2.normal = (inverse_transpose * math::Vec4{v2.normal.x(),v2.normal.y(),v2.normal.z(),0.0}).xyz().normalize();


        assert(_material != nullptr);
        auto tri = std::make_shared<geometry::Tri>(v0,v1,v2,_material,_vertexNormals);
        triangles.emplace_back(tri);
    }
    return triangles;
}

std::shared_ptr<materials::Material> radiance::geometry::TriMesh::getMaterial()
{
    return _material;
}

void radiance::geometry::TriMesh::getFaceVertices(uint32_t faceIndex, Vertex &v0, Vertex &v1, Vertex &v2) const
{
    uint32_t face_id = 3*faceIndex;
    v0 = _vertices[_indices[face_id]];
    v1 = _vertices[_indices[face_id + 1]];
    v2 = _vertices[_indices[face_id + 2]];

}

uint32_t radiance::geometry::TriMesh::getNumFaces() const
{
    return _numFaces;
}
