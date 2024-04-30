#include <iostream>


#include "io/image.h"
#include "io/mesh.h"
#include "io/log.h"
#include "math/vec.h"
#include "math/ray.h"
#include "math/util.h"
#include "math/transform.h"


#include "cameras/perspective.h"

#include "materials/diffuse.h"
#include "materials/mirror.h"
#include "materials/glass.h"
#include "materials/metal.h"
#include "materials/plastic.h"
#include "materials/diffuse_emitter.h"
#include "materials/texture.h"

#include "geometry/hit.h"
#include "geometry/sphere.h"
#include "geometry/triangle.h"
#include "geometry/scene.h"

#include "settings.h"


#include "argh/argh.h"


radiance::geometry::Scene scene0(){
    //Materials
    auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.5,0.5,0.5});
    auto floor = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.5,0.5,0.5});

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

radiance::geometry::Scene scene5(){
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
    //auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.2,0.2});
    auto right = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{0.8,0.6,0.2});
    auto floor = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.1,0.2,0.5});

    //Geometry
    

    std::vector<radiance::geometry::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    std::string filename = MODEL_DIR + std::string{"bunny.obj"};

    if(!radiance::io::readTriangleListFromObj(vertices,indices,filename.c_str())){
        std::cerr << "Failed to load obj: " << filename << std::endl;
    }else{
        std::cout << "Loaded obj file: " << filename << std::endl;
    }

    std::cout << "NUM FACES: " << indices.size() / 3 << std::endl;
    radiance::geometry::HitList objects{vertices,indices,center,radiance::math::Vec3{0.2,-0.5,-2}};


    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(-1.0,0,-1), 0.5,left));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(1.0,0,-1), 0.5,right));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects),true};

    return scene;
}

radiance::geometry::Scene scene6(){
    //Materials
    auto left = std::make_shared<radiance::materials::Glass>(1.5);

    std::string texture_path = TEXTURE_DIR + std::string{"grid.exr"};
    auto image = std::make_shared<radiance::io::Image<radiance::math::Color3>>();
    std::cout << texture_path << std::endl;
    if(!radiance::io::readRGBImageFromEXR(*image,texture_path.c_str())){
        std::cerr << "Failed to load: " << texture_path << std::endl;
    }

    auto texture = std::make_shared<radiance::materials::ImageTexture>(image);
    
    auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.2,0.2});
    auto right = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{0.8,0.6,0.2});
    auto floor = std::make_shared<radiance::materials::Metal>(radiance::math::Color3{0.1,0.2,0.5},0.3);

    //Geometry
    

    std::vector<radiance::geometry::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    std::string filename = MODEL_DIR + std::string{"bunny.obj"};

    if(!radiance::io::readTriangleListFromObj(vertices,indices,filename.c_str())){
        std::cerr << "Failed to load obj: " << filename << std::endl;
    }else{
        std::cout << "Loaded obj file: " << filename << std::endl;
    }

    std::cout << "NUM FACES: " << indices.size() / 3 << std::endl;

    auto transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{0.75,0.75,0.75})
        .rotate(radiance::math::degToRad(45),radiance::math::Vec3{0,1,0})
        .translate(radiance::math::Vec3{0.0,-0.5,-2.});

    radiance::geometry::HitList objects{vertices,indices,center,transform};


    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(-1.0,0,-1), 0.5,left));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(1.0,0,-1), 0.5,right));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects),true};

    return scene;
}

radiance::geometry::Scene scene7(){
    //Materials
    auto left = std::make_shared<radiance::materials::Glass>(1.5);


    
    auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.2,0.2});
    auto right = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{0.8,0.6,0.2});
    auto floor = std::make_shared<radiance::materials::Plastic>(radiance::math::Color3{0.2,0.2,0.8});

    //Geometry
    

    std::vector<radiance::geometry::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    std::string filename = MODEL_DIR + std::string{"bunny.obj"};

    if(!radiance::io::readTriangleListFromObj(vertices,indices,filename.c_str())){
        std::cerr << "Failed to load obj: " << filename << std::endl;
    }else{
        std::cout << "Loaded obj file: " << filename << std::endl;
    }

    std::cout << "NUM FACES: " << indices.size() / 3 << std::endl;

    auto transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{0.75,0.75,0.75})
        .rotate(radiance::math::degToRad(45),radiance::math::Vec3{0,1,0})
        .translate(radiance::math::Vec3{0.0,-0.5,-2.});

    radiance::geometry::HitList objects{vertices,indices,center,transform};


    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(-1.0,0,-1), 0.5,left));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(1.0,0,-1), 0.5,right));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects),true};

    return scene;
}

radiance::geometry::Scene scene8(){
    //Materials
    auto left = std::make_shared<radiance::materials::Glass>(1.5);

    auto light = std::make_shared<radiance::materials::DiffuseEmitter>(radiance::math::Color3{1,1,1}*20);
    
    auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.2,0.2});
    auto right = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{0.8,0.6,0.2});
    auto floor = std::make_shared<radiance::materials::Plastic>(radiance::math::Color3{0.2,0.2,0.8});

    //Geometry
    

    std::vector<radiance::geometry::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    std::string filename = MODEL_DIR + std::string{"bunny.obj"};

    if(!radiance::io::readTriangleListFromObj(vertices,indices,filename.c_str())){
        std::cerr << "Failed to load obj: " << filename << std::endl;
    }else{
        std::cout << "Loaded obj file: " << filename << std::endl;
    }

    std::cout << "NUM FACES: " << indices.size() / 3 << std::endl;

    auto transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{0.75,0.75,0.75})
        .rotate(radiance::math::degToRad(45),radiance::math::Vec3{0,1,0})
        .translate(radiance::math::Vec3{0.0,-0.5,-2.});

    radiance::geometry::HitList objects{vertices,indices,center,transform};


    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(-1.0,0,-1), 0.5,left));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(1.0,0,-1), 0.5,right));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,2.,-5), 1.0,light));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(0,-100.5,-1), 100,floor));

  
    //Construct scene
    radiance::geometry::Scene scene{std::move(objects),true,radiance::math::BLACK};

    return scene;
}

radiance::geometry::Scene scene9(){
    //Materials

    auto left = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{1.0,0.0,0.0});
    auto center = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{1.0,1.0,1.0});
    auto right = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.0,1.0,0.0});

    auto mirror = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{1,1,1});
    auto glass = std::make_shared<radiance::materials::Glass>(1.5);


    auto light = std::make_shared<radiance::materials::DiffuseEmitter>(radiance::math::Color3{1,1,1}*20);

    //Geometry
    

    std::vector<radiance::geometry::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    std::string filename = MODEL_DIR + std::string{"bunny.obj"};

    if(!radiance::io::readTriangleListFromObj(vertices,indices,filename.c_str())){
        std::cerr << "Failed to load obj: " << filename << std::endl;
    }else{
        std::cout << "Loaded obj file: " << filename << std::endl;
    }

    std::cout << "NUM FACES: " << indices.size() / 3 << std::endl;

    auto transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{0.75,0.75,0.75})
        .rotate(radiance::math::degToRad(45),radiance::math::Vec3{0,1,0})
        .translate(radiance::math::Vec3{0.0,-0.5,-2.});

    radiance::geometry::HitList bunny_mesh{vertices,indices,center,transform};

    filename = MODEL_DIR + std::string{"rect.obj"};
    vertices.clear();
    indices.clear();

    if(!radiance::io::readTriangleListFromObj(vertices,indices,filename.c_str())){
        std::cerr << "Failed to load obj: " << filename << std::endl;
    }else{
        std::cout << "Loaded obj file: " << filename << std::endl;
    }

    std::cout << "NUM FACES: " << indices.size() / 3 << std::endl;

    float dz = -3.0;

    transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{1,1,1}*10)
        .rotate(radiance::math::degToRad(0.001),radiance::math::Vec3{1,0,0})
        .translate(radiance::math::Vec3{0,0,-10 + dz});

    radiance::geometry::HitList back_mesh{vertices,indices,center,transform};

    transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{1,1,1}*10)
        .rotate(radiance::math::degToRad(89.99),radiance::math::Vec3{1,0,0})
        .translate(radiance::math::Vec3{0,-4.8,-5 + dz});

    radiance::geometry::HitList floor_mesh{vertices,indices,center,transform};

    transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{1,1,1}*10)
        .rotate(radiance::math::degToRad(-89.99),radiance::math::Vec3{1,0,0})
        .translate(radiance::math::Vec3{0,4.8,-5 + dz});

    radiance::geometry::HitList roof_mesh{vertices,indices,center,transform};

    transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{1,1,1}*10)
        .rotate(radiance::math::degToRad(89.99),radiance::math::Vec3{0,1,0})
        .translate(radiance::math::Vec3{-5,0,-5 + dz});

    radiance::geometry::HitList left_mesh{vertices,indices,left,transform};

    transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{1,1,1}*10)
        .rotate(radiance::math::degToRad(-89.99),radiance::math::Vec3{0,1,0})
        .translate(radiance::math::Vec3{5,0,-5 + dz});

    radiance::geometry::HitList right_mesh{vertices,indices,right,transform};

    transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{1,1,1}*3)
        .rotate(radiance::math::degToRad(89),radiance::math::Vec3{1,0,0})
        .translate(radiance::math::Vec3{0,4.8,-5 + dz});

    radiance::geometry::HitList light_mesh{vertices,indices,light,transform};


    radiance::geometry::HitList objects{};
    objects.mergeList(back_mesh);
    objects.mergeList(floor_mesh);
    objects.mergeList(roof_mesh);
    objects.mergeList(left_mesh);
    objects.mergeList(right_mesh);
    objects.mergeList(light_mesh);


    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(2.0,-3.0,-8), 1.5,mirror));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3(-2.0,-3.0,-8), 1.5,glass));


    //objects.mergeList(bunny_mesh);

    //Construct scene
    radiance::geometry::Scene scene{std::move(objects),true,radiance::math::BLACK};

    return scene;
}

radiance::geometry::Scene scene10(){
    //Materials
    auto left = std::make_shared<radiance::materials::Glass>(1.5);

    std::string texture_path = TEXTURE_DIR + std::string{"earth.png"};
    auto image = std::make_shared<radiance::io::Image<radiance::math::Color3>>();
    std::cout << texture_path << std::endl;
    if(!radiance::io::readRGBImageFromPNG(*image,texture_path.c_str())){
        std::cerr << "Failed to load: " << texture_path << std::endl;
    }

    auto texture = std::make_shared<radiance::materials::ImageTexture>(image);
    
    auto plastic = std::make_shared<radiance::materials::Plastic>(texture);

    auto mirror = std::make_shared<radiance::materials::Mirror>(radiance::math::Color3{1,1,1});
    auto green = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.2,0.8,0.2});
    auto red = std::make_shared<radiance::materials::Diffuse>(radiance::math::Color3{0.8,0.2,0.2});


    auto glass = std::make_shared<radiance::materials::Glass>(1.5);

    //Geometry
    

    std::vector<radiance::geometry::Vertex> vertices{};
    std::vector<uint32_t> indices{};

    std::string filename = MODEL_DIR + std::string{"bunny.obj"};

    if(!radiance::io::readTriangleListFromObj(vertices,indices,filename.c_str())){
        std::cerr << "Failed to load obj: " << filename << std::endl;
    }else{
        std::cout << "Loaded obj file: " << filename << std::endl;
    }

    std::cout << "NUM FACES: " << indices.size() / 3 << std::endl;

    auto transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{0.75,0.75,0.75})
        .rotate(radiance::math::degToRad(45),radiance::math::Vec3{0,1,0})
        .translate(radiance::math::Vec3{0.0,-0.5,-2.});

    radiance::geometry::HitList bunny_mesh{vertices,indices,green,transform};

    filename = MODEL_DIR + std::string{"rect.obj"};
    vertices.clear();
    indices.clear();

    if(!radiance::io::readTriangleListFromObj(vertices,indices,filename.c_str())){
        std::cerr << "Failed to load obj: " << filename << std::endl;
    }else{
        std::cout << "Loaded obj file: " << filename << std::endl;
    }

    std::cout << "NUM FACES: " << indices.size() / 3 << std::endl;


    transform = radiance::math::Transform{}
        .scale(radiance::math::Vec3{2,1,4}*100)
        .rotate(radiance::math::degToRad(89.99),radiance::math::Vec3{1,0,0})
        .translate(radiance::math::Vec3{0,-1.5,-5});

    radiance::geometry::HitList floor_mesh{vertices,indices,red,transform};

    radiance::geometry::HitList objects{};
    
    objects.mergeList(floor_mesh);
    //objects.mergeList(bunny_mesh);

    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3{3,-0.5,-4},1.0,glass));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3{0,0,-4},1.5,plastic));
    objects.addObject(std::make_shared<radiance::geometry::Sphere>(radiance::math::Vec3{-3,-0.5,-4},1.0,green));


    //Construct scene
    radiance::geometry::Scene scene{std::move(objects),true};

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
    }else if(sceneNum == 5){
        return scene5();
    }else if(sceneNum == 6){
        return scene6();
    }else if(sceneNum == 7){
        return scene7();
    }else if(sceneNum == 8){
        return scene8();
    }else if(sceneNum == 9){
        return scene9();
    }else if(sceneNum == 10){
        return scene10();
    }
    return scene0();
}



int main(int argc,char** argv){

    auto cmdl = argh::parser(argc, argv);

    int scene_num = 0;
    cmdl({"-s","--scene"},0) >> scene_num;

    auto scene = loadScene(scene_num);

    radiance::math::initRandom();



    //Image
    float aspect = 16.0/9.0;
    int width = 400;
    int height = static_cast<int>(width / aspect);
    int spp = 10;
    int max_rays = 300;
    float bounce_offset = 0.001;
    radiance::io::Image<radiance::math::Color3> out_image{width,height};

    //Camera
    float focal_length = 1.0;
    float view_height = 2.0; 
    float view_width = view_height * (float(width)/height);

    auto center = radiance::math::Vec3{0,0,0};
    auto view_u = radiance::math::Vec3{view_width,0,0};
    auto view_v = radiance::math::Vec3{0,-view_height,0};
    auto du = view_u / width;
    auto dv = view_v / height;
    auto top_left = center - radiance::math::Vec3{0,0,focal_length} - view_u*0.5 - view_v*0.5;
    auto p00 = top_left + du*0.5 + dv*0.5;


    //radiance::cameras::Perspective camera{view_width,view_height,width,height};

    auto camera = radiance::cameras::Perspective::lookAt(
        aspect,
        90.0,
        width,
        radiance::math::Vec3{0,0.0,1.5},
        radiance::math::Vec3{0,0,-1}
    );

    scene.render(out_image,camera,spp,max_rays,bounce_offset);

    
   

    std::string out_name = OUTPUT_DIR + std::string{"out_"} + std::to_string(scene_num) + std::string{".png"};
    if(!radiance::io::writeRGBImageToPNG(out_image,out_name.c_str())){
        std::cerr << "uh oh\n";
    }

    return 0;
}