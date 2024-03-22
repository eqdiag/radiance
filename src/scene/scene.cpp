#include "scene.h"

#include <optional>
#include <cassert>
#include "math/util.h"
#include "math/rand.h"


radiance::scene::Scene::Scene(cameras::PerspectiveCamera &camera)
    :_camera{camera}
{

}

void radiance::scene::Scene::generateImageBuffer(char *pixels) const
{
    for(int i = 0;i < _camera.viewingPlane.imageHeight;i++){
        for(int j = 0;j < _camera.viewingPlane.imageWidth;j++){
    
            math::Color3 col{};
            for(int k = 0; k < _camera.viewingPlane.spp;k++){

                float dx = math::randomFloat(-0.5,0.5f);
                float dy = math::randomFloat(-0.5,0.5f);


                auto ray = _camera.generateRay(j+dx,i+dy);
                auto current_col = shade(ray);


                col += current_col;
            }
            col /= _camera.viewingPlane.spp;


            float r = std::max(0.0f,col.r());
            float g = std::max(0.0f,col.g());
            float b = std::max(0.0f,col.b());


            int idx = _camera.viewingPlane.numChannels*(i*_camera.viewingPlane.imageWidth + j);
            pixels[idx] = r*255.99;
            pixels[idx + 1] = g*255.99;
            pixels[idx + 2] = b*255.99;
        }
        std::cout << "Rendered line: [" << i + 1<< "/" << _camera.viewingPlane.imageHeight << "]\n"; 
    }
}

math::Color3 radiance::scene::Scene::shade(const math::Ray &ray) const
{
    //Check if you hit something
    geometry::Hit hit{};
    if(trace(ray,hit,0.001)){
        assert(hit._material != nullptr);
        //Check if the material causes the ray to bounce
        math::Color3 attenuation{};
        math::Vec3 out_dir{};
        //If so, shade recursively
        std::cout << "mat?\n";
        if(hit._material->bounce(ray.getDir(),hit,attenuation,out_dir)){
            math::Ray bounce_ray{hit.point,out_dir};
            std::cout << "here?\n";
            return attenuation * shade(bounce_ray);
        }else{
            std::cout << "here2?\n";

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

bool radiance::scene::Scene::trace(const math::Ray &ray, geometry::Hit &hit, float tmin, float tmax) const
{
    std::optional<geometry::Hit> closest{};

    for(const auto& object: _objects){
        geometry::Hit current{};
        if(object->trace(ray,current,tmin,tmax)){
            if(closest.has_value()){
                //If there was already a close object, check if something else is closer
                if(current.t < closest.value().t){
                    closest = current;
                    assert(closest.value()._material != nullptr);
                }
                //Otherwise, do nothing
            }else{
                //If no objects seen so far, set current to closest
                closest = current;
                assert(closest.value()._material != nullptr);
            }
        }
    }

    bool hit_found = closest.has_value();
    if(hit_found){
        hit = closest.value();
        assert(hit._material != nullptr);
    }

    return hit_found;
}

math::Color3 radiance::scene::Scene::backgroundColor(const math::Ray &ray) const
{
    if(_backgroundColor.has_value()){
        return _backgroundColor.value();
    }
    float y = 0.5*(ray.getDir().y() + 1.0);;
    return math::Color3{0.5,0.7,1.0}*(1.0 - y) + math::Color3{1,1,1}*y;
}

void radiance::scene::Scene::addObject(std::shared_ptr<geometry::Hittable> object)
{
    _objects.push_back(object);
}

void radiance::scene::Scene::addLight(const lights::PointLight &light)
{
    _lights.emplace_back(light);
}

void radiance::scene::Scene::setCamera(cameras::PerspectiveCamera &camera)
{
    _camera = camera;
}

void radiance::scene::Scene::setBackgroundColor(math::Color3 &color)
{
    _backgroundColor = color;
}

void radiance::scene::Scene::validate() const
{
    for(auto& obj: _objects){
        assert(obj->_material != nullptr);
    }
}
