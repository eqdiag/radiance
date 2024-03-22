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

    /*Enable/Disable scene parser debug message*/    
    bool debug;
    debug = cmdl[{ "-d", "--debug" }];

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




    math::initRandom();

    

 

    /* Parse scene file*/

    radiance::misc::Timer timer{"Scene parse"};

    radiance::scene::Scene scene{};
    radiance::io::SceneParser parser{};


    timer.start();
    if(parser.readSceneFromFile(scene,scene_file.c_str(),debug)){
        std::cout << "Loaded scene file: " << scene_file << std::endl;
    }else{
        std::cerr << "ERROR: Failed to load scene file: " << scene_file << std::endl;
        return 0;
    }

    timer.end();
    timer.displaySeconds();

    do{
        std::cout << "Press enter to render scene...\n";
    }while(std::cin.get() != '\n');


    auto view_plane = parser.getViewingPlane();
    auto w = view_plane.imageWidth;
    auto h = view_plane.imageHeight;
    auto num_channels = view_plane.numChannels;

    int buffer_size = w * h * num_channels;
    char pixels[buffer_size];
    scene.generateImageBuffer(pixels);

    if(!radiance::io::writeBufferToRGB_PNG(pixels,w,h,filename.c_str())){
        std::cerr << "\n Failed to write image: " << filename << std::endl;
    }else{
        std::cout << "\n Wrote rendered image: " << filename << std::endl;
    }
    
    std::cout << "Max depth traversed: " << scene.getMaxDepth() << std::endl;
}