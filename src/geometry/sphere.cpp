#include "sphere.h"

radiance::geometry::Sphere::Sphere(math::Vec3 center, float radius):
    _center{center},
    _radius{radius}
{

}

bool radiance::geometry::Sphere::trace(const math::Ray& ray,Hit& hit,float tmin,float tmax) const
{
    
    auto dx = ray.p - _center;

    float a = 1.0;
    float b = 2.0 * ray.v.dot(dx);
    float c = dx.dot(dx) - _radius*_radius;

    float discrim = b*b - 4*a*c;
    if(discrim < 0){
        return false;
    }

    float t = (-b - std::sqrt(discrim)) * 0.5;
    if((t > tmin) && (tmax > t)){
        hit.t = t;
        hit.p = ray.cast(t);
        auto n = (hit.p - _center) / _radius;
        hit.setNormal(ray,n);
        return true;
    }

    t = (-b + std::sqrt(discrim)) * 0.5;
    if((t > tmin) && (tmax > t)){
        hit.t = t;
        hit.p = ray.cast(t);
        auto n = (hit.p - _center) / _radius;
        hit.setNormal(ray,n);
        return true;
    }

    return false;    
}
