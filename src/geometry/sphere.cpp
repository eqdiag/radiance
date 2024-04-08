#include "sphere.h"

#include "math/util.h"

radiance::geometry::Sphere::Sphere(math::Vec3 center, float radius,std::shared_ptr<materials::Material> material):
    _center{center},
    _radius{radius},
    material{material},
    box{
        _center - math::Vec3{radius,radius,radius},
        _center + math::Vec3{radius,radius,radius}
    }
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
        hit.material = material;
        auto uv = pointToUV(hit.p);
        hit.u = uv.x();
        hit.v = uv.y();
        return true;
    }

    t = (-b + std::sqrt(discrim)) * 0.5;
    if((t > tmin) && (tmax > t)){
        hit.t = t;
        hit.p = ray.cast(t);
        auto n = (hit.p - _center) / _radius;
        hit.setNormal(ray,n);
        hit.material = material;
        auto uv = pointToUV(hit.p);
        hit.u = uv.x();
        hit.v = uv.y();
        return true;
    }

    return false;    
}

bool radiance::geometry::Sphere::boundingBox(geometry::AABB &outBox) const
{
    outBox = box;
    return true;
}

radiance::math::Vec2 radiance::geometry::Sphere::pointToUV(const math::Vec3 &p) const
{
    float phi = acos(p.y());
    float theta = atan2(-p.z(),p.x()) + math::PI;
    return math::Vec2{theta / math::TWO_PI,phi / math::PI};
}
