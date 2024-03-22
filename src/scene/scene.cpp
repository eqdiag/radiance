#include "scene.h"

#include <optional>
#include "math/util.h"
#include "math/rand.h"
#include <cassert>

int radiance::scene::Scene::_maxDepth = 0;

radiance::scene::Scene::Scene()
{
    
}

radiance::scene::Scene::Scene(cameras::PerspectiveCamera &camera)
    : _camera{camera}
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
                auto current_col = shade(ray,0);


                col += current_col;
            }

            col /= _camera.viewingPlane.spp;


            //TODO: Tonemapping here, loss of spectrum due to tonemapping here
            float r = std::clamp(col.r(),0.0f,1.0f);
            float g = std::clamp(col.g(),0.0f,1.0f);
            float b = std::clamp(col.b(),0.0f,1.0f);

            //apply gamma correction
            r = std::pow(r,1.0/2.2);
            g = std::pow(g,1.0/2.2);
            b = std::pow(b,1.0/2.2);


            int idx = _camera.viewingPlane.numChannels*(i*_camera.viewingPlane.imageWidth + j);
            pixels[idx] = r*255.99;
            pixels[idx + 1] = g*255.99;
            pixels[idx + 2] = b*255.99;
        }
        std::cout << "\rRendered line: [" << i + 1<< "/" << _camera.viewingPlane.imageHeight << "]";
        std::cout << std::flush;
       
    }
}

math::Color3 radiance::scene::Scene::shade(const math::Ray &ray,int depth) const
{
    _maxDepth = (depth > _maxDepth) ? depth : _maxDepth;

    if(depth == MAX_DEPTH){
        return backgroundColor(ray);
    }

    //Check if you hit something
    geometry::Hit hit{};
    //std::cout << "before\n";
    if(trace(ray,hit,0.01)){
        //Check if the material causes the ray to bounce
        math::Color3 attenuation{};
        math::Vec3 out_dir{};
        //If so, shade recursively
        if(hit._material->bounce(ray.getDir(),hit,attenuation,out_dir)){
            math::Ray bounce_ray{hit.point,out_dir};
            return attenuation * shade(bounce_ray,depth+1);
        }else{
        
        //Otherwise no bounce, so compute direct lighting... for now
            math::Color3 color{};
            for(const auto& light: _lights){

                //std::cout << "l col: " << light.intensity << std::endl;

                auto l = (light.position - hit.point);
                float d = l.norm();

                //Check if pt in shadow
                math::Ray shadow_ray{hit.point,l};
                geometry::Hit shadow_hit{};
                bool some_hit = trace(shadow_ray,shadow_hit,0.001);

                bool in_shadow = some_hit && (shadow_hit.t < d);

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

void radiance::scene::Scene::clearObjects()
{
    _objects.clear();
}

void radiance::scene::Scene::clearLights()
{
    _lights.clear();
}

void radiance::scene::Scene::setCamera(cameras::PerspectiveCamera &camera)
{
    _camera = camera;
}

void radiance::scene::Scene::setBackgroundColor(math::Color3 &color)
{
    _backgroundColor = color;
}

int radiance::scene::Scene::getMaxDepth() const
{
    return _maxDepth;
}
