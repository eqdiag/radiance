#include "scene.h"



radiance::geometry::Scene::Scene(HitList && objects):
    objects{objects}
{

}

bool radiance::geometry::Scene::trace(const math::Ray &ray, Hit &hit, float tmin, float tmax) const
{
    return objects.trace(ray,hit,tmin,tmax);
}

void radiance::geometry::Scene::render(io::Image<math::Color3> &image,const cameras::Perspective& camera, int samplesPerPixel, int maxBounces, float bounceOffset)
{
    //Render loop
    for(int i = 0;i < image.height;i++){
        for(int j= 0;j< image.width;j++){

            //auto film_pt = corner + dx*j + dy*i;

            //auto ray = radiance::math::Ray{eye,(film_pt - eye)};
            auto ray = camera.generateRay(i,j);

            auto final_color = radiance::math::BLACK;

            for(int k = 0;k < samplesPerPixel;k++){
                auto color = background(ray);
                radiance::geometry::Hit hit{};
                if(objects.trace(ray,hit)){
                    color = (hit.n+radiance::math::Vec3{1,1,1})*.5;
                }
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
