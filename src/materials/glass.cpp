#include "glass.h"
#include "math/util.h"

radiance::materials::Glass::Glass(float refractionIndex):
    Material{math::WHITE},
    refractionIndex{refractionIndex}
{

}

bool radiance::materials::Glass::bounce(const math::Ray &in, const geometry::Hit &hit, math::Color3 &attenutation, math::Ray &out) const
{
    

    //Going from in to out, or out to in
    float ratio = hit.outside ? 1.0/refractionIndex : refractionIndex;

    float cos = - in.v.dot(hit.n);
    float sin = sqrt(1.0 - cos);

    //Compute fraction of reflected light
    float f = math::schlick(cos,ratio);
    
    //Check for total internal reflection
    bool no_refract = (sin*ratio > 1.0f);

    //Reflect if you have to (from TIL) or some fraction of the time based on reflectivity
    if(no_refract || (math::randomFloat() < f)){
        out = math::Ray{hit.p,math::reflect(in.v,hit.n)};
        attenutation = texture->getColor(hit);
    }else{
        out = math::Ray{hit.p,math::refract(in.v,hit.n,ratio)};
        attenutation = texture->getColor(hit);
    }
    
    return true;
}
