#include "hit.h"
#include <cassert>

void radiance::geometry::Hit::setFaceNormal(const math::Vec3 &in, const math::Vec3 &n)
{
    outside = in.dot(n) < 0.0f;
    normal = outside ? n : -n;
}

void radiance::geometry::Hittable::setMaterial(std::shared_ptr<materials::Material> material)
{
    _material = material;
}

radiance::geometry::InstancedHittable::InstancedHittable(std::shared_ptr<Hittable> object, math::Transform transform):
_object{object},
_transform{transform}
{

}

bool radiance::geometry::InstancedHittable::trace(const math::Ray &ray, Hit &hit, float tmin, float tmax) const
{
    auto inverse = _transform.getInverseMatrix();
    auto old_p = ray.getSrc();
    auto old_v = ray.getDir();

    auto new_p = (inverse * math::Vec4{old_p.x(),old_p.y(),old_p.z(),1.0}).xyz();
    auto new_v = (inverse * math::Vec4{old_v.x(),old_v.y(),old_v.z(),0.0}).xyz();

    math::Ray new_ray{new_p,new_v};
    
    if(!_object->trace(new_ray,hit,tmin,tmax)) return false;

    //Otherwise, if hit, then make sure to update p and normals
    auto matrix = _transform.getMatrix();
    auto inverse_transpose = inverse.transpose();

    hit.point = (matrix * math::Vec4{hit.point.x(),hit.point.y(),hit.point.z(),1.0}).xyz();
    hit.normal = (inverse_transpose * math::Vec4{hit.normal.x(),hit.normal.y(),hit.normal.z(),0.0}).xyz().normalize();
   

    hit._material = _material;

    return true;
}
