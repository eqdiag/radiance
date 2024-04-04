#include <iostream>


#include "io/image.h"
#include "io/log.h"
#include "math/vec.h"
#include "math/ray.h"
#include "math/util.h"


#include "cameras/perspective.h"

#include "geometry/hit.h"
#include "geometry/sphere.h"

#include "geometry/scene.h"


#include "settings.h"



radiance::math::Color3 background(const radiance::math::Ray& ray){
    float t = 0.5*(ray.v.y() + 1.0);
    return radiance::math::WHITE*(1.0f - t) + radiance::math::Color3(0.5, 0.7, 1.0)*t;
}

int main(){

    radiance::math::initRandom();


    //Image
    float aspect = 16.0/9.0;
    int width = 400;
    int height = static_cast<int>(width / aspect);
    int spp = 10;
    int max_bounces = 100;
    float bounce_offset = 0.0001;
    radiance::io::Image<radiance::math::Color3> out_image{width,height};

    //Camera 
    //auto eye = radiance::math::Vec3::ZERO();
    //auto front = radiance::math::Vec3{0,0,-1};

    float focal_length = 1.0;
    float view_height = 2.0; //Specify height because vfov
    float view_width = view_height * aspect;

    /*auto view_u = radiance::math::Vec3{view_width,0,0};
    auto view_v = radiance::math::Vec3{0,-view_height,0};

    //Dimensions of each pixel
    auto dx = view_u / width;
    auto dy = view_v / height;

    auto corner = eye + front - view_u*0.5 - view_v*0.5;

    corner += dx*0.5 + dy*0.5;*/
    

    radiance::cameras::Perspective camera{view_width,view_height,width,height};


    //Geometry
    
    radiance::geometry::HitList objects{};
    auto sphere = std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3{0,0,-1.0},0.5);

    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100));
    objects.addObject(sphere);

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects)};

    scene.render(out_image,camera,spp,max_bounces,bounce_offset);


    radiance::io::writeRGBImageToPPM(out_image,"out.ppm");

    return 0;
}