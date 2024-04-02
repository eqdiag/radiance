#include "mesh.h"

#include "tiny_obj/tiny_obj_loader.h"
#include "math/vec.h"

#include "happly/happly.h"

#include <cassert>


bool radiance::io::readTriMeshFromObj(geometry::TriMesh &mesh, const char *filename)
{
    tinyobj::ObjReaderConfig config{};
    tinyobj::ObjReader reader{};

    if(!reader.ParseFromFile(filename)){
        return false;
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();


    std::vector<radiance::geometry::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    bool use_vertex_normals = false;

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {

    
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            assert(fv == 3);

            // Loop over vertices in the face.
            std::vector<math::Vec3> face_vertices{};

            for (size_t v = 0; v < fv; v++) {

                radiance::geometry::Vertex new_vertex{};


                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];


                new_vertex.position = math::Vec3{vx,vy,vz};

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                    if(!use_vertex_normals) use_vertex_normals = true;
                    new_vertex.normal = math::Vec3{nx,ny,nz};
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    new_vertex.uv = math::Vec2{tx,1.0f-static_cast<float>(ty)};
                }
                vertices.emplace_back(new_vertex);

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
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

    mesh.setVertices(std::move(vertices));
    mesh.setIndices(std::move(indices));

    if(use_vertex_normals) mesh.enableVertexNormals();

    return true;
}

bool radiance::io::readTriMeshFromPly(geometry::TriMesh &mesh, const char *filename)
{
    happly::PLYData ply_input(filename);

    std::vector<float> xs = ply_input.getElement("vertex").getProperty<float>("x");
    std::vector<float> ys = ply_input.getElement("vertex").getProperty<float>("y");
    std::vector<float> zs = ply_input.getElement("vertex").getProperty<float>("z");

    assert(xs.size() == ys.size());
    assert(ys.size() == zs.size());

    std::vector<radiance::geometry::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    auto temp_indices = ply_input.getFaceIndices();
    for(const auto& face: temp_indices){
        if(face.size() != 3){
            std::cerr << "PLY loader only supports triangulated meshes!\n";
            return false;
        }
        
        indices.push_back(face[0]);
        indices.push_back(face[1]);
        indices.push_back(face[2]);
        
        /*vertices.emplace_back(math::Vec3{xs[face[0]],ys[face[0]],zs[face[0]]});
        vertices.emplace_back(math::Vec3{xs[face[1]],ys[face[1]],zs[face[1]]});
        vertices.emplace_back(math::Vec3{xs[face[2]],ys[face[2]],zs[face[2]]});*/

    }

    mesh.setVertices(std::move(vertices));
    mesh.setIndices(std::move(indices));


    return true;
}

bool radiance::io::readTriMeshFromFile(geometry::TriMesh &mesh, const char *filename, MeshFileType fileType)
{
    if(fileType == MeshFileType::OBJ){
        return readTriMeshFromObj(mesh,filename);
    }else{
        return readTriMeshFromPly(mesh,filename);
    }
}
