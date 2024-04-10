#include "triangle.h"

radiance::geometry::Triangle::Triangle(Vertex &p0, Vertex &p1, Vertex &p2,std::shared_ptr<materials::Material> material):
    v0{p0},
    v1{p1},
    v2{p2},
    v01{v1.p - v0.p},
    v12{v2.p - v1.p},
    v20{v0.p - v2.p},
    faceNormal{(p1.p - p0.p).cross(p2.p - p0.p).normalize()},
    area{(p1.p - p0.p).cross(p2.p - p0.p).norm()*0.5f},
    material{material},
    box{
        p0.p.min(p1.p).min(p2.p),
        p0.p.max(p1.p).max(p2.p),
    }
{
}

bool radiance::geometry::Triangle::trace(const math::Ray &ray, Hit &hit, float tmin, float tmax) const
{
    //First check for plane intersection
    float denom = ray.v.dot(faceNormal);
    if(std::abs(denom) < 0.00001) return false;

    float t = -(ray.p - v0.p).dot(faceNormal) / denom;
    if((t <= tmin)  || (t >= tmax)) return false;

    
    math::Vec3 q = ray.cast(t);


    //Now check if plane pt is inside the triangle
    auto q0 = q - v0.p;
    auto q1 = q - v1.p;
    auto q2 = q - v2.p;


    float check0 = v01.cross(q0).dot(faceNormal);
    float check1 = v12.cross(q1).dot(faceNormal);
    float check2 = v20.cross(q2).dot(faceNormal);

    bool valid = ((check0 < 0) && (check1 < 0) && (check2 < 0));
    valid |= ((check0 > 0) && (check1 > 0) && (check2 > 0));

    if(!valid) return false;
    hit.t = t;
    hit.p = q;



    //Compute barycentric coords
    float a2 = (0.5*v01.cross(q0).norm()) / area;
    float a0 = (0.5*v12.cross(q1).norm()) / area;
    float a1 = (0.5*v20.cross(q2).norm()) / area;

    hit.uv = v0.uv*a0 + v1.uv*a1 + v2.uv*a2;

    /*auto normal = _normal;
    if(_vertexNormals){
        //std::cout << "n\n";
       
        normal = _p0.normal*a0 + _p1.normal*a1 + _p2.normal*a2;
        normal = normal.normalize();
    }*/

    hit.setNormal(ray,faceNormal);

    hit.material = material;


    return true;
}

bool radiance::geometry::Triangle::boundingBox(geometry::AABB &outBox) const
{
    outBox = box;
    return true;
}

std::shared_ptr<radiance::materials::Material> radiance::geometry::Triangle::getMaterial() const
{
    return material;
}
