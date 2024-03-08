#include "scene.h"

#include <optional>
#include "math/util.h"

math::Color3 scene::Scene::shade(const math::Ray &ray)
{
    //Check if you hit something
    geometry::Hit hit{};
    if(trace(ray,hit,0.001)){

        //Check if the material causes the ray to bounce
        math::Color3 attenuation{};
        math::Vec3 out_dir{};
        //If so, shade recursively
        if(hit._material->bounce(ray.getDir(),hit,attenuation,out_dir)){
            math::Ray bounce_ray{hit.point,out_dir};
            return attenuation * shade(bounce_ray);
        }else{
        //Otherwise no bounce, so compute direct lighting... for now
            math::Color3 color{};
            for(const auto& light: _lights){

                auto l = (light.position - hit.point);
                float d = l.norm();

                //Check if pt in shadow
                math::Ray shadow_ray{hit.point,l};
                geometry::Hit shadow_hit{};
                bool in_shadow = trace(shadow_ray,shadow_hit,0.001);

                //Only add light contribution if pt not in shadow
                math::Color3 col{};
                if(!in_shadow){
                    auto light_dir = l / d;
                    col = light.intensity * std::max(0.0f,hit.normal.dot(light_dir));
                    col /= (d*d * math::PI);
                }
                color += col;
            }
            color *= attenuation;
            return color;
        }
    }

    return backgroundColor(ray);
}

bool scene::Scene::trace(const math::Ray &ray, geometry::Hit &hit, float tmin, float tmax) const
{
    std::optional<geometry::Hit> closest{};

    for(const auto& object: _objects){
        geometry::Hit current{};
        if(object->trace(ray,current,tmin,tmax)){
            if(closest.has_value()){
                //If there was already a close object, check if something else is closer
                if(current.t < closest.value().t){
                    closest = current;
                }
                //Otherwise, do nothing
            }else{
                //If no objects seen so far, set current to closest
                closest = current;
            }
        }
    }

    bool hit_found = closest.has_value();
    if(hit_found){
        hit = closest.value();
    }

    return hit_found;
}

math::Color3 scene::Scene::backgroundColor(const math::Ray &ray)
{
   float y = 0.5*(ray.getDir().y() + 1.0);;
   return math::Color3{0.5,0.7,1.0}*(1.0 - y) + math::Color3{1,1,1}*y;
}

void scene::Scene::addObject(std::shared_ptr<geometry::Hittable> object)
{
    _objects.push_back(object);
}

void scene::Scene::addLight(const lights::PointLight &light)
{
    _lights.emplace_back(light);
}
