#include "scene.h"
#include "materials/material.h"
#include "acceleration/bvh.h"

radiance::geometry::Scene::Scene(HitList&& hitList,bool buildBVH,std::optional<math::Color3> backgroundColor):
    backgroundColor{backgroundColor}
{   
    if(buildBVH){
        sceneRoot = std::make_shared<acceleration::BVHNode>(hitList.objects);
    }else{
        sceneRoot = std::make_shared<geometry::HitList>(std::move(hitList));
    }

}

radiance::math::Color3 radiance::geometry::Scene::radiance(const math::Ray &ray, int depth, float bounceOffset)
{
    if(depth <= 0){
        return math::BLACK;
    }


    geometry::Hit hit{};
    //Check if ray hits something
    if(sceneRoot->trace(ray,hit,bounceOffset)){

        math::Color3 color = math::BLACK;

        //TODO: Add emittance
        color += hit.material->emit(hit);




        //Multi-bounce loop
        math::Color3 reflectance{};
        math::Ray out{};
        if(hit.material->bounce(ray,hit,reflectance,out)){
            color += reflectance * radiance(out,depth-1,bounceOffset);
        }
        return color;
    }

    if(backgroundColor.has_value()){
        return backgroundColor.value();
    }else{
        return background(ray);
    }
}

void radiance::geometry::Scene::render(io::Image<math::Color3> &image,const cameras::Perspective& camera, int samplesPerPixel, int maxBounces, float bounceOffset)
{
    //Render loop
    for(int i = 0;i < image.height;i++){
        for(int j= 0;j< image.width;j++){

    
            auto ray = camera.generateRay(i,j);

            auto final_color = radiance::math::BLACK;

            for(int k = 0;k < samplesPerPixel;k++){
                auto color = radiance(ray,maxBounces,bounceOffset);
                final_color += color;
            }
            final_color /= static_cast<float>(samplesPerPixel);


                        
            image.write(i,j,final_color);
    
        }
        std::cout << "\rRendered line: [" << i + 1 << "/" << image.height << "]";
        std::cout << std::flush;
    }
    std::cout << "\n";
}

radiance::math::Color3 radiance::geometry::Scene::background(const radiance::math::Ray& ray) const
{
    float t = 0.5*(ray.v.y() + 1.0);
    return radiance::math::WHITE*(1.0f - t) + radiance::math::Color3(0.5, 0.7, 1.0)*t;
}
