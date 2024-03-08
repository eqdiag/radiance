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

    //Samples per pixel (anti-aliasing)
    int spp;
    cmdl("-spp",1) >> spp;
    std::cout << spp << std::endl;

    /* Outfile param */
    std::string out_file{};
    cmdl({"-o","--out"},"mirror.png") >> out_file;
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
 
    cameras::PerspectiveCamera camera{viewing_plane,vfov,lookFrom,lookAt};

    /* Materials */



    scene::Scene scene{};

    /* Objects */

    auto blue_material = std::make_shared<materials::Diffuse>(math::Color3{0,0,1});
    auto red_material = std::make_shared<materials::Diffuse>(math::Color3{1,0,0});
    auto mirror_material = std::make_shared<materials::Mirror>();

    auto center = math::Vec3{0,0,-1};
    auto sphere0 = std::make_shared<geometry::Sphere>(center,0.5f,mirror_material);
    center = math::Vec3{0.0f,-100.0f,0.0f};
    auto sphere1 = std::make_shared<geometry::Sphere>(center,99.5f,red_material);

    scene.addObject(sphere0);
    scene.addObject(sphere1);


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