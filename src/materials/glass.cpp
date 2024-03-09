#include "glass.h"

#include "math/util.h"
#include "math/rand.h"

materials::Glass::Glass(float indexOfRefraction):
    _index{indexOfRefraction}
{
}

bool materials::Glass::bounce(const math::Vec3 &in, const geometry::Hit &hit, math::Vec3 &attenuation, math::Vec3 &out) const
{
    attenuation = math::Vec3{1,1,1};
    float ratio = hit.outside ? 1.0 /_index : _index;

    //First check if we can refract
    float cos = - in.dot(hit.normal);
    float sin = sqrt(1.0f - cos*cos);
    bool cant_refract = (sin * ratio) > 1.0f;

    
    if(cant_refract){
        out = math::reflect(in,hit.normal);
    }else{
        //Refract
        out = math::refract(in,hit.normal,ratio);
    }
    return true;
}
