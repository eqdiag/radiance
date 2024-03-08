#include "sphere.h"

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
        hit.normal = (hit.point - _center) / _radius;
        //Adjust so that normal always faces away from ray dir
        hit.normal = hit.normal.dot(ray.getDir()) > 0.0f ? -hit.normal : hit.normal;
        hit._material = _material;
        return true;
    }
    t = (-b + std::sqrt(discrim)) * 0.5;
    if((t > tmin) && (tmax > t)){
        hit.t = t;
        hit.point = ray.cast(t);
        hit.normal = (hit.point - _center) / _radius;
        //Adjust so that normal always faces away from ray dir
        hit.normal = hit.normal.dot(ray.getDir()) > 0.0f ? -hit.normal : hit.normal;
        hit._material = _material;
        return true;
    }

    return false;
}
