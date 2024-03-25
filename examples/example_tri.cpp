#include <iostream>


#include "settings.h"

#include "io/image.h"
#include "geometry/sphere.h"
#include "math/vec.h"
#include "math/util.h"
#include "cameras/perspective_camera.h"
#include "scene/scene.h"
#include "lights/point_light.h"
#include "math/rand.h"

#include "materials/diffuse.h"
#include "materials/mirror.h"
#include "materials/glass.h"

#include "geometry/tri.h"

#include "argh/argh.h"

#include "pugixml/src/pugixml.hpp"

int main(int argc,char ** argv){

    auto cmdl = argh::parser(argc, argv);

    
    /* Viewing plane params */
    int w;
    cmdl({"-w","--width"},600) >> w;

    int h;
    cmdl({"-h","--height"},400) >> h;

    float focalLength;
    cmdl({"-f","--focal"},1.0f) >> focalLength;
    std::cout << focalLength << std::endl;

    float lensRadius;
    cmdl({"-r","--radius"},0.0f) >> lensRadius;
    std::cout << lensRadius << std::endl;

    /* Fov params */
    float vfov;
    cmdl("-vfov",90.0f) >> vfov;

    //Samples per pixel (anti-aliasing)
    int spp;
    cmdl("-spp",1) >> spp;
    std::cout << spp << std::endl;

    /* Outfile param */
    std::string out_file{};
    cmdl({"-o","--out"},"example_tri.png") >> out_file;
    std::string filename = OUTPUT_DIR + out_file;


    /* Camera params */

    math::Vec3 lookFrom{};
    cmdl("-lookfrom","[0,0,0]") >> lookFrom;

    math::Vec3 lookAt{};
    cmdl("-lookAt","[0,0,-1]") >> lookAt;

    math::initRandom();

    int channels = 3;
    char pixels[w*h*channels];


    cameras::ViewingPlane viewing_plane{};
    viewing_plane.aspectRatio = static_cast<float>(w)/h;
    viewing_plane.imageHeight = h;
    viewing_plane.imageWidth = w;
    viewing_plane.focalLength = focalLength;
    viewing_plane.spp = spp;
 
    cameras::PerspectiveCamera camera{viewing_plane,vfov,lensRadius,lookFrom,lookAt};

    /* Materials */



    radiance::scene::Scene scene{camera};

    /* Objects */


    auto blue_material = std::make_shared<materials::Diffuse>(math::Color3{0,0,1});

    auto tri = std::make_shared<geometry::Tri>(math::Vec3{0,1,-3},math::Vec3{-1,-1,-3},math::Vec3{1,-1,-3},blue_material);

 
    scene.addObject(tri);


    lights::PointLight light{};
    light.position = math::Vec3{3,3,0};
    light.intensity = math::Vec3{1,1,1}*30.0;

    scene.addLight(light);

    for(int i = 0;i < h;i++){
        for(int j = 0;j < w;j++){
    
            math::Color3 col{};
            for(int k = 0; k < spp;k++){

                float dx = math::randomFloat(-0.5,0.5f);
                float dy = math::randomFloat(-0.5,0.5f);


                auto ray = camera.generateRay(j+dx,i+dy);

                auto current_col = scene.shade(ray);

                col += current_col;
            }
            col /= spp;


            float r = std::max(0.0f,col.r());
            float g = std::max(0.0f,col.g());
            float b = std::max(0.0f,col.b());


            int idx = channels*(i*w + j);
            pixels[idx] = r*255.99;
            pixels[idx + 1] = g*255.99;
            pixels[idx + 2] = b*255.99;
        }
    }

    if(!radiance::io::writeBufferToRGB_PNG(pixels,w,h,filename.c_str())){
        std::cerr << "failed to write image: " << filename << std::endl;
    }else{
        std::cout << "wrote rendered image: " << filename << std::endl;
    }
    
}