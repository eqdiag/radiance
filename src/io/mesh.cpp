#include "mesh.h"
#include "tiny_obj/tiny_obj_loader.h"

#include "geometry/triangle.h"

#include <cassert>


bool radiance::io::readTriangleListFromObj(std::vector<geometry::Vertex> &vertices, std::vector<uint32_t> &indices, const char *filename)
{
    tinyobj::ObjReaderConfig config{};
    tinyobj::ObjReader reader{};

    if(!reader.ParseFromFile(filename)){
        return false;
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();



    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {

    
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            assert(fv == 3);


            for (size_t v = 0; v < fv; v++) {

                radiance::geometry::Vertex new_vertex{};

                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                new_vertex.p = math::Vec3{vx,vy,vz};

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                    new_vertex.n = math::Vec3{nx,ny,nz};
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    new_vertex.uv = math::Vec2{tx,1.0f-static_cast<float>(ty)};
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];


                vertices.push_back(new_vertex);
            }

            
            uint32_t size = static_cast<uint32_t>(indices.size());
            indices.push_back(size);
            indices.push_back(size+1);
            indices.push_back(size+2);

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }

    }

    return true;
}
