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

#include "geometry/tri_mesh.h"

#include "io/mesh.h"
#include "io/scene.h"

#include "misc/timer.h"


int main(int argc,char ** argv){

    auto cmdl = argh::parser(argc, argv);

    
    /* Viewing plane params */
    int w;
    cmdl({"-w","--width"},600) >> w;

    int h;
    cmdl({"-h","--height"},400) >> h;

    float focalLength;
    cmdl({"-f","--focal"},1.0f) >> focalLength;
    //std::cout << focalLength << std::endl;

    float lensRadius;
    cmdl({"-r","--radius"},0.0f) >> lensRadius;
    //std::cout << lensRadius << std::endl;

    /* Fov params */
    float vfov;
    cmdl("-vfov",90.0f) >> vfov;

    //Samples per pixel (anti-aliasing)
    int spp;
    cmdl("-spp",1) >> spp;
    //std::cout << spp << std::endl;

    /*Scene file param*/
    std::string scene_file{};
    cmdl({"-s","--scene"},"none") >> scene_file;
    if(!strcmp(scene_file.c_str(),"none")){
        std::cerr << "ERROR: No scene file specified\n";
        return 0;
    }
    scene_file = SCENE_DIR + scene_file;


    /* Outfile param */
    std::string out_file{};
    cmdl({"-o","--out"},"out.png") >> out_file;
    std::string filename = OUTPUT_DIR + out_file;


    /* Camera params */

    math::Vec3 lookFrom{};
    cmdl("-lookfrom","[0,0,4]") >> lookFrom;

    math::Vec3 lookAt{};
    cmdl("-lookAt","[0,0,-1]") >> lookAt;

    math::initRandom();

    /* Default Camera setup*/


    cameras::ViewingPlane viewing_plane{};
    viewing_plane.aspectRatio = static_cast<float>(w)/h;
    viewing_plane.imageHeight = h;
    viewing_plane.imageWidth = w;
    viewing_plane.focalLength = focalLength;
    viewing_plane.spp = spp;
    viewing_plane.numChannels = 3;
 
    cameras::PerspectiveCamera camera{viewing_plane,vfov,lensRadius,lookFrom,lookAt};

    /* Parse scene file*/

    radiance::misc::Timer timer{"Scene parse"};

    radiance::scene::Scene scene{camera};
    radiance::io::SceneParser parser{};


    timer.start();
    if(parser.readSceneFromFile(scene,scene_file.c_str())){
        std::cout << "Loaded scene file: " << scene_file << std::endl;
    }else{
        std::cerr << "ERROR: Failed to load scene file: " << scene_file << std::endl;
        return 0;
    }

    scene.validate();

    timer.end();
    timer.displaySeconds();



   
    //TODO FIX THIS!!!!!, use parsed values
    int buffer_size = parser.getBufferSize();
    char pixels[buffer_size];

    scene.generateImageBuffer(pixels);
    

    if(!radiance::io::writeBufferToRGB_PNG(pixels,w,h,filename.c_str())){
        std::cerr << "failed to write image: " << filename << std::endl;
    }else{
        std::cout << "wrote rendered image: " << filename << std::endl;
    }
    
}