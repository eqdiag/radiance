#include <iostream>


#include "settings.h"

#include "io/image.h"
#include "geometry/sphere.h"
#include "math/vec.h"
#include "math/util.h"
#include "cameras/perspective_camera.h"
#include "scene/scene.h"

#include "materials/diffuse.h"

#include "argh/argh.h"

int main(int argc,char ** argv){

    auto cmdl = argh::parser(argc, argv);

    /* Viewing plane params */
    int w;
    cmdl({"-w","--width"},256) >> w;

    int h;
    cmdl({"-h","--height"},256) >> h;

    float focalLength;
    cmdl({"-f","--focal"},1.0f) >> focalLength;

    /* Fov params */
    float vfov;
    cmdl("-vfov",90.0f) >> vfov;


    /* Outfile param */
    std::string out_file{};
    cmdl({"-o","--out"},"example_many_spheres.png") >> out_file;
    std::string filename = OUTPUT_DIR + out_file;

    /* Camera params */

    math::Vec3 lookFrom{};
    cmdl("-lookfrom","[0,0,0]") >> lookFrom;

     math::Vec3 lookAt{};
    cmdl("-lookAt","[0,0,-1]") >> lookAt;



    int channels = 3;
    char pixels[w*h*channels];
  

    cameras::ViewingPlane viewing_plane{};
    viewing_plane.aspectRatio = static_cast<float>(w)/h;
    viewing_plane.imageHeight = h;
    viewing_plane.imageWidth = w;
    viewing_plane.focalLength = focalLength;
 
    cameras::PerspectiveCamera camera{viewing_plane,vfov,0.0f,lookFrom,lookAt};


    radiance::scene::Scene scene{camera};

    auto blue_material = std::make_shared<materials::Diffuse>(math::Color3{0,0,1});

    auto center = math::Vec3{0,0,-2};
    auto sphere0 = std::make_shared<geometry::Sphere>(center,0.5f,blue_material);
    center = math::Vec3{0,0,-10};
    auto sphere1 = std::make_shared<geometry::Sphere>(center,4.0f,blue_material);
    center = math::Vec3{0,0,-20};
    auto sphere2 = std::make_shared<geometry::Sphere>(center,10.0f,blue_material);

    scene.addObject(sphere0);
    scene.addObject(sphere1);
    scene.addObject(sphere2);



    for(int i = 0;i < h;i++){
        for(int j = 0;j < w;j++){
    

            auto ray = camera.generateRay(j,i);

            geometry::Hit hit{};
            math::Color3 col{};
            if(scene.trace(ray,hit)){
                col = (hit.normal + math::Vec3{1,1,1})*0.5;
            }else{
                col = math::Color3{0,0,0};
            }

            float r = std::max(0.0f,col.r());
            float g = std::max(0.0f,col.g());
            float b = std::max(0.0f,col.b());


            int idx = channels*(i*w + j);
            pixels[idx] = r*256;
            pixels[idx + 1] = g*256;
            pixels[idx + 2] = b*256;
        }
    }

    if(!radiance::io::writeBufferToRGB_PNG(pixels,w,h,filename.c_str())){
        std::cerr << "failed to write image: " << filename << std::endl;
    }else{
        std::cout << "wrote rendered image: " << filename << std::endl;
    }
    
}