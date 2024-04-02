#include "sphere.h"
#include "math/util.h"

#include <cassert>

radiance::geometry::Sphere::Sphere(math::Vec3 & center, float radius):
    _center{center},
    _radius{radius}
{
    computeBoundingBox();
}

radiance::geometry::Sphere::Sphere(math::Vec3 &center, float radius,std::shared_ptr<materials::Material> material):
    _center{center},
    _radius{radius}
{
    _material = material;
    computeBoundingBox();
}

void radiance::geometry::Sphere::computeBoundingBox()
{
    _box = radiance::geometry::AABB{
        _center - math::Vec3{_radius,_radius,_radius},
        _center + math::Vec3{_radius,_radius,_radius},
    };
}

bool radiance::geometry::Sphere::trace(const math::Ray& ray,Hit &hit,float tmin,float tmax) const 
{

    assert(_material != nullptr);
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
        hit.uv = pointToUV(normal);

        return true;
    }
    t = (-b + std::sqrt(discrim)) * 0.5;
    if((t > tmin) && (tmax > t)){
        hit.t = t;
        hit.point = ray.cast(t);
        auto normal = (hit.point - _center) / _radius;
        //Adjust so that normal always faces away from ray dir
        hit.setFaceNormal(ray.getDir(),normal);
        //hit.uv = pointToUV(normal);
        hit._material = _material;
        assert(hit._material != nullptr);
        return true;
    }

    return false;
}

bool radiance::geometry::Sphere::getBoundingBox(radiance::geometry::AABB &box) const
{
    box = _box;
    return false;
}

math::Vec2 radiance::geometry::Sphere::pointToUV(const math::Vec3 &p) const
{
    float phi = acos(p.y());
    float theta = atan2(-p.z(),p.x()) + math::PI;
    return math::Vec2{theta / math::TWO_PI,phi / math::PI};
}
