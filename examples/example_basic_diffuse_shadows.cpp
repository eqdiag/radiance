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
    cmdl({"-o","--out"},"example_basic_diffuse_shadows.png") >> out_file;
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
 
    cameras::PerspectiveCamera camera{viewing_plane,vfov,0.0f,lookFrom,lookAt};


    scene::Scene scene{};

    auto blue_material = std::make_shared<materials::Diffuse>(math::Color3{0,0,1});


    auto center = math::Vec3{0,0,-1};
    auto sphere0 = std::make_shared<geometry::Sphere>(center,0.5f,blue_material);
    center = math::Vec3{0.0f,-100.0f,0.0f};
    auto sphere1 = std::make_shared<geometry::Sphere>(center,99.5f,blue_material);

    scene.addObject(sphere0);
    scene.addObject(sphere1);

    math::Vec3 sphere_albedo{1.0,1.0,1.0};


    lights::PointLight light{};
    light.position = math::Vec3{3,3,0};
    light.intensity = math::Vec3{1,1,1}*40.0f;

    for(int i = 0;i < h;i++){
        for(int j = 0;j < w;j++){
    

           
            math::Color3 col{};
            for(int k = 0; k < spp;k++){

                float dx = math::randomFloat(-0.5,0.5f);
                float dy = math::randomFloat(-0.5,0.5f);


                auto ray = camera.generateRay(j+dx,i+dy);

                geometry::Hit hit{};
                math::Color3 current_col{};
                if(scene.trace(ray,hit)){

                    auto l = (light.position - hit.point);
                    float d = l.norm();


                    math::Ray new_ray{hit.point,l}; 
                    geometry::Hit new_hit{};


                    bool in_shadow = scene.trace(new_ray,new_hit,0.001);
                    
                    if(!in_shadow){
                        auto light_dir = l / d;
                        current_col = sphere_albedo * light.intensity * std::max(0.0f,hit.normal.dot(light_dir));
                        current_col /= (d*d * math::PI);
                    }

                    //col = (hit.normal + math::Vec3{1,1,1})*0.5;
                }else{
                    current_col = math::Color3{0,0,0};
                }

                col += current_col;
            }
            col /= spp;

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