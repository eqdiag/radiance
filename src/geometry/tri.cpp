#include "tri.h"
#include "materials/diffuse.h"
#include <memory>
#include <vector>

radiance::geometry::Tri::Tri(const math::Vec3 &p0, const math::Vec3 &p1, const math::Vec3 &p2,std::shared_ptr<materials::Material> material):
    _p0{p0},
    _p1{p1},
    _p2{p2},
    _p01{p1 - p0},
    _p12{p2 - p1},
    _p20{p0 - p2},
    _normal{(p1 - p0).cross(p2 - p0).normalize()},
    _material{material}
{
    computeBoundingBox();
}

void radiance::geometry::Tri::computeBoundingBox()
{
    _box = radiance::geometry::AABB{
        _p0.min(_p1).min(_p2),
        _p0.max(_p1).max(_p2)
    };
}

bool radiance::geometry::Tri::trace(const math::Ray &ray, Hit &hit, float tmin, float tmax) const
{

    //First check for plane intersection
    float denom = ray.getDir().dot(_normal);
    if(std::abs(denom) < 0.0001) return false;

    float t = -(ray.getSrc() - _p0).dot(_normal) / denom;
    if((t <= tmin)  || (t >= tmax)) return false;

    
    math::Vec3 q = ray.cast(t);


    //Now check if plane pt is inside the triangle
    auto q0 = q - _p0;
    auto q1 = q - _p1;
    auto q2 = q - _p2;


    float check0 = _p01.cross(q0).dot(_normal);
    float check1 = _p12.cross(q1).dot(_normal);
    float check2 = _p20.cross(q2).dot(_normal);

    bool valid = ((check0 < 0) && (check1 < 0) && (check2 < 0));
    valid |= ((check0 > 0) && (check1 > 0) && (check2 > 0));

    if(!valid) return false;
    hit.t = t;
    hit.point = q;
    hit.setFaceNormal(ray.getDir(),_normal);
    hit._material = _material;


    return true;
}

bool radiance::geometry::Tri::getBoundingBox(radiance::geometry::AABB &box) const
{
    box = _box.pad(0.01);
    return true;
}

