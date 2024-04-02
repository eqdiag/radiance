#include "tri.h"
#include "materials/diffuse.h"
#include <memory>
#include <vector>

radiance::geometry::Tri::Tri(const Vertex &p0, const Vertex &p1, const Vertex &p2,std::shared_ptr<materials::Material> material,bool vertexNormals):
    _p0{p0},
    _p1{p1},
    _p2{p2},
    _p01{p1.position - p0.position},
    _p12{p2.position - p1.position},
    _p20{p0.position - p2.position},
    _area{_p01.cross(-_p20).norm()*0.5f},
    _normal{(p1.position - p0.position).cross(p2.position - p0.position).normalize()},
    _material{material},
    _vertexNormals{vertexNormals}
{
    computeBoundingBox();
    
}

float radiance::geometry::Tri::getArea() const
{
    return _area;
}

void radiance::geometry::Tri::computeBoundingBox()
{
    _box = radiance::geometry::AABB{
        _p0.position.min(_p1.position).min(_p2.position),
        _p0.position.max(_p1.position).max(_p2.position)
    };
}

bool radiance::geometry::Tri::trace(const math::Ray &ray, Hit &hit, float tmin, float tmax) const
{
    //First check for plane intersection
    float denom = ray.getDir().dot(_normal);
    if(std::abs(denom) < 0.0001) return false;

    float t = -(ray.getSrc() - _p0.position).dot(_normal) / denom;
    if((t <= tmin)  || (t >= tmax)) return false;

    
    math::Vec3 q = ray.cast(t);


    //Now check if plane pt is inside the triangle
    auto q0 = q - _p0.position;
    auto q1 = q - _p1.position;
    auto q2 = q - _p2.position;


    float check0 = _p01.cross(q0).dot(_normal);
    float check1 = _p12.cross(q1).dot(_normal);
    float check2 = _p20.cross(q2).dot(_normal);

    bool valid = ((check0 < 0) && (check1 < 0) && (check2 < 0));
    valid |= ((check0 > 0) && (check1 > 0) && (check2 > 0));

    if(!valid) return false;
    hit.t = t;
    hit.point = q;



    //Compute barycentric coords
    float a2 = (0.5*_p01.cross(q0).norm()) / _area;
    float a0 = (0.5*_p12.cross(q1).norm()) / _area;
    float a1 = (0.5*_p20.cross(q2).norm()) / _area;

    hit.uv = _p0.uv*a0 + _p1.uv*a1 + _p2.uv*a2;

    auto normal = _normal;
    if(_vertexNormals){
        //std::cout << "n\n";
       
        normal = _p0.normal*a0 + _p1.normal*a1 + _p2.normal*a2;
        normal = normal.normalize();
    }

    hit.setFaceNormal(ray.getDir(),normal);


    hit._material = _material;


    return true;
}

bool radiance::geometry::Tri::getBoundingBox(radiance::geometry::AABB &box) const
{
    box = _box.pad(0.01);
    return true;
}

