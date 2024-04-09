#include "hit.h"
#include "geometry/triangle.h"

#include <optional>

void radiance::geometry::Hit::setNormal(const radiance::math::Ray &ray, const radiance::math::Vec3 normal)
{
    outside = ray.v.dot(normal) < 0.0f;
    n = outside ? normal : -normal;
}

radiance::geometry::HitList::HitList(){

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
