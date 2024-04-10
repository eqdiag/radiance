#include "hit.h"
#include "geometry/triangle.h"

#include <optional>

void radiance::geometry::Hit::setNormal(const radiance::math::Ray &ray, const radiance::math::Vec3 normal)
{
    outside = ray.v.dot(normal) < 0.0f;
    n = outside ? normal : -normal;
}

std::shared_ptr<radiance::materials::Material> radiance::geometry::Hittable::getMaterial() const{
    return nullptr;
}


radiance::geometry::HitList::HitList()
{

}

radiance::geometry::HitList::HitList(HitList &&rhs):
    objects{rhs.objects}
{
    
}

void radiance::geometry::HitList::addObject(std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
}

bool radiance::geometry::HitList::boundingBox(geometry::AABB &box) const
{
    //TODO
    return true;
}



radiance::geometry::HitList::HitList(const std::vector<geometry::Vertex> &vertices, const std::vector<uint32_t> &indices,std::shared_ptr<materials::Material> material,math::Vec3 offset)
{
    uint32_t num_faces = indices.size()/3;
    for(uint32_t f = 0;f < num_faces;f++){
        auto v0 = vertices[indices[3*f]];
        v0.p += offset;
        auto v1 = vertices[indices[3*f + 1]];
        v1.p += offset;
        auto v2 = vertices[indices[3*f + 2]];        
        v2.p += offset;
        objects.push_back(
            std::make_shared<geometry::Triangle>(v0,v1,v2,material)
        );
    }
}

void radiance::geometry::HitList::mergeList(const HitList& list)
{  
    for(const auto& obj: list.objects){
        objects.push_back(obj);
    }
}

radiance::geometry::HitList::HitList(const std::vector<geometry::Vertex>& vertices,const std::vector<uint32_t>& indices,std::shared_ptr<materials::Material> material,math::Transform transform)
{
    auto matrix = transform.matrix;
    auto inverse_transpose = transform.inverse.transpose();

    uint32_t num_faces = indices.size()/3;
    for(uint32_t f = 0;f < num_faces;f++){
        auto v0 = vertices[indices[3*f]];
        v0.p = (matrix * math::Vec4{v0.p.x(),v0.p.y(),v0.p.z(),1.0}).xyz();
        v0.n = (inverse_transpose * math::Vec4{v0.n.x(),v0.n.y(),v0.n.z(),0.0}).xyz().normalize();

        auto v1 = vertices[indices[3*f + 1]];
        v1.p = (matrix * math::Vec4{v1.p.x(),v1.p.y(),v1.p.z(),1.0}).xyz();
        v1.n = (inverse_transpose * math::Vec4{v1.n.x(),v1.n.y(),v1.n.z(),0.0}).xyz().normalize();

        auto v2 = vertices[indices[3*f + 2]];        
        v2.p = (matrix * math::Vec4{v2.p.x(),v2.p.y(),v2.p.z(),1.0}).xyz();
        v2.n = (inverse_transpose * math::Vec4{v2.n.x(),v2.n.y(),v2.n.z(),0.0}).xyz().normalize();

        auto triangle = std::make_shared<geometry::Triangle>(v0,v1,v2,material);

        objects.push_back(
            triangle
        );
    }
}


bool radiance::geometry::HitList::trace(const math::Ray& ray,Hit& hit,float tmin,float tmax) const{

    std::optional<Hit> closest{};

    for(const auto& obj: objects){
        Hit new_hit{};
        if(obj->trace(ray,new_hit,tmin,tmax)){
            if(closest.has_value()){
                if(new_hit.t < closest.value().t){
                    closest = new_hit;
                }
            }else{
                closest = new_hit;
            }
        }
    }

    if(closest.has_value()){
        hit = closest.value();
        return true;
    }

    return false;
}


