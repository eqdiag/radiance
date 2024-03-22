#include "sphere.h"
#include <cassert>

geometry::Sphere::Sphere(math::Vec3 & center, float radius):
    _center{center},
    _radius{radius}
{
    
}

geometry::Sphere::Sphere(math::Vec3 &center, float radius,std::shared_ptr<materials::Material> material):
    _center{center},
    _radius{radius},
    _material{material}
{

}

bool geometry::Sphere::trace(const math::Ray& ray,Hit &hit,float tmin,float tmax) const 
{
    auto dx = ray.getSrc() - _center;

    float a = 1.0;
    float b = 2.0 * ray.getDir().dot(dx);
    float c = dx.dot(dx) - _radius*_radius;

    float discrim = b*b - 4*a*c;
    if(discrim < 0){
        return false;
    }

    float t = (-b - std::sqrt(discrim)) * 0.5;
    if((t > tmin) && (tmax > t)){
        hit.t = t;
        hit.point = ray.cast(t);
        auto normal = (hit.point - _center) / _radius;
        //Adjust so that normal always faces away from ray dir
        hit.setFaceNormal(ray.getDir(),normal);
        hit._material = _material;
        assert(hit._material != nullptr);
        return true;
    }
    t = (-b + std::sqrt(discrim)) * 0.5;
    if((t > tmin) && (tmax > t)){
        hit.t = t;
        hit.point = ray.cast(t);
        auto normal = (hit.point - _center) / _radius;
        //Adjust so that normal always faces away from ray dir
        hit.setFaceNormal(ray.getDir(),normal);
        hit._material = _material;
        assert(hit._material != nullptr);
        return true;
    }

    return false;
}
