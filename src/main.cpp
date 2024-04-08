#include <iostream>


#include "io/image.h"
#include "io/log.h"
#include "math/vec.h"
#include "math/ray.h"
#include "math/util.h"


#include "cameras/perspective.h"

#include "materials/diffuse.h"
#include "materials/mirror.h"
#include "materials/glass.h"
#include "materials/texture.h"

#include "geometry/hit.h"
#include "geometry/sphere.h"
#include "geometry/scene.h"

#include "settings.h"


#include "argh/argh.h"


radiance::geometry::Scene scene0(){
    //Materials
    auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.7,0.3,0.3});
    auto floor = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.8,0.0});

    //Geometry
    
    radiance::geometry::HitList objects{};

    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,0,-1), 0.5,center));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects)};

    return scene;
}

radiance::geometry::Scene scene1(){
    //Materials
    auto left = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{0.8,0.8,0.8});
    auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.7,0.3,0.3});
    auto right = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{0.8,0.6,0.2});

    auto floor = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.8,0.0});

    //Geometry
    
    radiance::geometry::HitList objects{};

    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(-1.0,0,-1), 0.5,left));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,0,-1), 0.5,center));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(1.0,0,-1), 0.5,right));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects)};

    return scene;
}

radiance::geometry::Scene scene2(){
    //Materials
    auto left = std::make_shared<radiance::materials::Glass>(1.5);
    auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.1,0.2,0.5});
    auto right = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{0.8,0.6,0.2});

    auto floor = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.8,0.0});

    //Geometry
    
    radiance::geometry::HitList objects{};

    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(-1.0,0,-1), 0.5,left));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,0,-1), 0.5,center));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(1.0,0,-1), 0.5,right));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects)};

    return scene;
}

radiance::geometry::Scene scene3(){
    //Materials
    
    auto floor = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.8,0.8});

    //Geometry
    
    radiance::geometry::HitList objects{};

    int sz = 10;

    for(int i = -sz;i< sz;i++){
        for(int j = -sz;j < sz;j++){
            float r = radiance::math::randomFloat();
            float g = radiance::math::randomFloat();
            float b = radiance::math::randomFloat();

            auto type = radiance::math::randInt(0,1);
            std::shared_ptr<radiance::materials::Material> material{};
            if(type == 0){
                material = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{r,g,b});
            }else{
                material = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{r,g,b});
            }

            float x = i*.2;
            float y = radiance::math::randomFloat(0,0.2);
            float z = j*.2 - 1.5;

            objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(x,y,z), 0.1,material));
        }
    }


    
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects),true};

    return scene;
}

radiance::geometry::Scene scene4(){
    //Materials
    auto left = std::make_shared<radiance::materials::Glass>(1.5);

    std::string texture_path = TEXTURE_DIR + std::string{"grid.exr"};
    auto image = std::make_shared<radiance::io::Image<radiance::math::Color3>>();
    std::cout << texture_path << std::endl;
    if(!radiance::io::readRGBImageFromEXR(*image,texture_path.c_str())){
        std::cerr << "Failed to load: " << texture_path << std::endl;
    }


    auto texture = std::make_shared<radiance::materials::ImageTexture>(image);
    

    auto center = std::make_shared<radiance::materials::Diffuse>(texture);
    auto right = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{0.8,0.6,0.2});
    auto floor = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.1,0.2,0.5});

    //Geometry
    
    radiance::geometry::HitList objects{};

    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(-1.0,0,-1), 0.5,left));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,0,-1), 0.5,center));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(1.0,0,-1), 0.5,right));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects)};

    return scene;
}

radiance::geometry::Scene loadScene(int sceneNum){
    if(sceneNum == 0){
        return scene0();
    }else if(sceneNum == 1){
        return scene1();
    }else if(sceneNum == 2){
        return scene2();
    }else if(sceneNum == 3){
        return scene3();
    }else if(sceneNum == 4){
        return scene4();
    }
    return scene0();
}

int main(int argc,char** argv){

    auto cmdl = argh::parser(argc, argv);

    int scene_num = 0;
    cmdl({"-s","--scene"},0) >> scene_num;


    radiance::math::initRandom();

    radiance::geometry::Sphere s{radiance::math::Vec3{0,0,0},1.0,nullptr};

    std::cout << s.pointToUV(radiance::math::Vec3{0,-1,1}.normalize()) << std::endl;



    //Image
    float aspect = 16.0/9.0;
    int width = 400;
    int height = static_cast<int>(width / aspect);
    int spp = 100;
    int max_rays = 100;
    float bounce_offset = 0.0001;
    radiance::io::Image<radiance::math::Color3> out_image{width,height};

    //Camera
    float focal_length = 1.0;
    float view_height = 2.0; //Specify height because vfov
    float view_width = view_height * aspect;

    //radiance::cameras::Perspective camera{view_width,view_height,width,height};

    auto camera = radiance::cameras::Perspective::lookAt(
        aspect,
        100.0,
        width,
        radiance::math::Vec3{0,.5,1},
        radiance::math::Vec3{0,0,-1}
    );


    std::cout << "Loading scene num: " << scene_num << std::endl;

    auto scene = loadScene(scene_num);
    
    radiance::math::Timer timer{"Render time"};
    timer.begin();
    scene.render(out_image,camera,spp,max_rays,bounce_offset);
    timer.stop();
    timer.displaySeconds();

    std::string out_name = OUTPUT_DIR + std::string{"out_"} + std::to_string(scene_num) + std::string{".png"};
    if(!radiance::io::writeRGBImageToPNG(out_image,out_name.c_str())){
        std::cerr << "uh oh\n";
    }

    return 0;
}