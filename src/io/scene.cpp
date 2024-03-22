#include "scene.h"
#include <sstream>
#include <string>
#include <cassert>

#include "materials/diffuse.h"
#include "materials/mirror.h"

radiance::io::SceneParser::SceneParser()
{
    //Inits sensible defaults
   
    _cameraParams.viewingPlane.focalLength = 1.0f;
    _cameraParams.viewingPlane.spp = 1;
    _cameraParams.viewingPlane.numChannels = 3;
    _cameraParams.fov = 90.0f;
    _cameraParams.lensRadius = 0.0f;
    _cameraParams.lookFrom = {};
    _cameraParams.lookAt = math::Vec3{0,0,-1};
    _cameraParams.up = math::Vec3{0,1,0};
}

bool radiance::io::SceneParser::readSceneFromFile(radiance::scene::Scene &scene, const char *fileName)
{

    pugi::xml_document doc{};
    if(doc.load_file(fileName)){
        auto scene_node = doc.child("scene");   

        for(auto& child: scene_node.children()){
            
            //Sensor node
            if(!strcmp(child.name(),"sensor")){
                if(!parseSceneSensorNode(scene,child)) return false;
            }

            //Bsdf node
            if(!strcmp(child.name(),"bsdf")){
                if(!parseSceneMaterialNode(scene,child)) return false;
            }

            //Parse geometry node
            if(!strcmp(child.name(),"shape")){
                if(!parseSceneGeometryNode(scene,child)) return false;
            }

            //Parse light node
            if(!strcmp(child.name(),"emitter")){
                if(!parseSceneLightNode(scene,child)) return false;
            }

            //Parse background node
            if(!strcmp(child.name(),"background")){
                if(!parseSceneBackgroundNode(scene,child)) return false;
            }
        }
    
        
      

    }else{
        return false;
    }

    //Do all the fancy parsing, then finally build

    if(!build(scene)) return false;

    return true;
}

int radiance::io::SceneParser::getBufferSize() const
{
    return _cameraParams.viewingPlane.imageWidth * _cameraParams.viewingPlane.imageHeight * _cameraParams.viewingPlane.numChannels;
}

bool radiance::io::SceneParser::parseSceneSensorNode(radiance::scene::Scene &scene, pugi::xml_node &sensor_node)
{
    std::cout << "PARSING SENSOR\n";

    auto cam_type = sensor_node.attribute("type").value();

    //Only support perspective cameras right now
    if(strcmp(cam_type,"perspective")) return false;


    for(auto& child: sensor_node.children()){

        //Parse fov type
        if(!strcmp(child.name(),"string")){
            auto name = child.attribute("name").value();
            auto value = child.attribute("y").value();
            std::cout << "\tPARSING FOV TYPE\n";
            if(!strcmp(name,"fovAxis")){
                if(!strcmp(value,"y")){
                    yfov = true;
                }else if(!strcmp(value,"x")){
                    yfov = false;
                    std::cout << "PARSE ERR: Don't currently support different non y-axis FOV!\n";
                    return false;
                }
            }
        }

        //Parse location info
        if(!strcmp(child.name(),"transform")){
            std::cout << "\tPARSING CAM INFO\n";
            auto lookat_node = child.child("lookAt");
            auto origin_node = lookat_node.attribute("origin").value();
            auto target_node = lookat_node.attribute("target").value();
            auto up_node = lookat_node.attribute("up").value();

            //Parse origin
            std::istringstream s(origin_node);
            s >> _cameraParams.lookFrom;
            //Parse target
            s = std::istringstream(target_node);
            s >> _cameraParams.lookAt;
            //Parse up
            s = std::istringstream(up_node);
            s >> _cameraParams.up;
        }

        //Parse fov value
        if(!strcmp(child.name(),"float")){
            std::cout << "\tPARSING FOV DATA\n";

            auto name = child.attribute("name").value();
            auto value = child.attribute("value").value();

            if(!strcmp(name,"fov")){
                _cameraParams.fov = std::stof(value);
            }

        }
    
        //Parse sampler
        if(!strcmp(child.name(),"sampler")){
            std::cout << "\tPARSING SAMPLER DATA\n";
            auto sampler_type = child.attribute("type").value();
            //Only supports independent sampler rn
            if(strcmp(sampler_type,"independent")){
                std::cout << "PARSE ERR: Don't currently support samplers other than independent!\n";
                return false;
            }
            
            auto sampler_data = child.child("integer");
            auto name = sampler_data.attribute("name").value();
            auto value = sampler_data.attribute("value").value();

            if(!strcmp(name,"sampleCount")){
                _cameraParams.viewingPlane.spp = std::stoi(value);
            }

        }

        //Parse image info
        if(!strcmp(child.name(),"film")){
            std::cout << "\tPARSING IMAGE DATA\n";

            auto film_type = child.attribute("type").value();

            if(!strcmp(film_type,"hdrfilm")){
                std::cout << "PARSE SOFT ERR: Don't actually support HDR rn but accept width/height info for PNG images!\n";
            }

            for(auto& sub_child: child.children()){

                auto dimension_name = sub_child.attribute("name").value();
                auto dimension_value = sub_child.attribute("value").value();

                if(!strcmp(dimension_name,"width")){
                    _cameraParams.viewingPlane.imageWidth = std::stoi(dimension_value);
                }else if(!strcmp(dimension_name,"height")){
                    _cameraParams.viewingPlane.imageHeight = std::stoi(dimension_value);
                }else{
                    return false;
                }

            }


        }

    }



    return true;
}

bool radiance::io::SceneParser::parseSceneMaterialNode(radiance::scene::Scene &scene, pugi::xml_node &material_node)
{
    std::cout << "PARSING MATERIAL\n";

    auto material_type = material_node.attribute("type").value();
    auto material_id = material_node.attribute("id").value();

    if(_materialMap.contains(material_id)){
        std::cout << "PARSE SOFT ERR: Duplicate material id ["<< material_id <<  "] found!\n";
    }

    if(!strcmp(material_type,"diffuse")){


        auto diffuse_data = material_node.child("rgb");
        auto name = diffuse_data.attribute("name").value();
        auto value = diffuse_data.attribute("value").value();
        if(strcmp(name,"reflectance")){ 
            std::cout << "PARSE ERR: Must have reflectance `name` on `rgb` node to be a valid scene file!\n";
            return false;
        }

        math::Color3 albedo{};
        std::istringstream s(value);
        s >> albedo;
        std::cout << "\tPARSING DIFFUSE: " << albedo;

       _materialMap[material_id] = std::make_shared<materials::Diffuse>(albedo);

    }else if(!strcmp(material_type,"mirror")){

        auto mirror_data = material_node.child("rgb");
        auto name = mirror_data.attribute("name").value();
        auto value = mirror_data.attribute("value").value();
        if(strcmp(name,"reflectance")){ 
            std::cout << "PARSE ERR: Must have reflectance `name` on `rgb` node to be a valid scene file!\n";
            return false;
        }

        math::Color3 reflectance{};
        std::istringstream s(value);
        s >> reflectance;
        std::cout << "\tPARSING MIRROR: " << reflectance;
        std::cout << "PARSE SOFT ERR: Don't currently support mirror materials with custom reflectance!\n";
       _materialMap[material_id] = std::make_shared<materials::Mirror>();

    }else{
        std::cerr << "PARSE ERR: Don't currently support material: " << material_type <<  "!\n";
        return false;
    }

    return true;
}

bool radiance::io::SceneParser::parseSceneLightNode(radiance::scene::Scene &scene, pugi::xml_node &light_node)
{
    std::cout << "PARSING LIGHT\n";

    auto light_type = light_node.attribute("type").value();

    //Only support point lights rn
    if(!strcmp(light_type,"point")){

        lights::PointLight light{};

        auto rgb_node = light_node.child("rgb");
        auto intensity = rgb_node.attribute("name").value();
        auto value = rgb_node.attribute("value").value();

        if(strcmp(intensity,"intensity")) return false;
        

        std::istringstream s(value);
        s >> light.intensity;

        auto point_node = light_node.child("point");
        auto name = point_node.attribute("position").value();
        float x = std::stof(point_node.attribute("x").value());
        float y = std::stof(point_node.attribute("y").value());
        float z = std::stof(point_node.attribute("z").value());

        light.position = math::Vec3{x,y,z};
        
        _lights.push_back(light);

        std::cout << "\tPARSING POINT LIGHT:\n";
        std::cout << "\t\tINTENSITY: " << light.intensity;
        std::cout << "\t\tPOSITION: " << light.position;


    }else{
        std::cerr << "PARSE ERR: Don't currently support light type: " << light_type <<  "!\n";
        return false;
    }

    return true;
}

bool radiance::io::SceneParser::parseSceneGeometryNode(radiance::scene::Scene &scene, pugi::xml_node &geometry_node)
{
    std::cout << "PARSING GEOMETRY\n";

    auto shape_type = geometry_node.attribute("type").value();

    //Parse obj
    if(!strcmp(shape_type,"obj")){
        std::cout << "\tPARSING OBJ\n";


    //Parse ply
    }else if(!strcmp(shape_type,"ply")){
        std::cout << "\tPARSING PLY\n";


    //Parse sphere
    }else if(!strcmp(shape_type,"sphere")){
        auto point_node = geometry_node.child("point");
        auto radius_node = geometry_node.child("float");
        auto id_node = geometry_node.child("ref");

        if(strcmp(point_node.attribute("name").value(),"center")) return false;

        if(strcmp(radius_node.attribute("name").value(),"radius")) return false;

    

        float x = std::stof(point_node.attribute("x").value());
        float y = std::stof(point_node.attribute("y").value());
        float z = std::stof(point_node.attribute("z").value());

        float r = std::stof(radius_node.attribute("value").value());

        auto material_id = id_node.attribute("id").value();
        
        math::Vec3 center{x,y,z};
        auto sphere = std::make_shared<geometry::Sphere>(center,r);
        _objects.emplace_back(HittableParams{sphere,material_id});
        std::cout << "\tPARSING SPHERE:\n";
        std::cout << "\t\tCENTER: " << center;
        std::cout << "\t\tRADIUS: " << r << std::endl;

    //Parse rect
    }else if(!strcmp(shape_type,"rect")){
        std::cout << "\tPARSING RECT\n";

    }else{

        std::cerr << "PARSE ERR: Don't currently support shape: " << shape_type <<  "!\n";
        return false;
    }


    return true;
}

bool radiance::io::SceneParser::parseSceneBackgroundNode(radiance::scene::Scene &scene, pugi::xml_node &bg_node)
{
    std::cout << "PARSING BACKGROUND\n";

    auto rgb_node = bg_node.child("rgb");
    auto value = rgb_node.attribute("value").value();

    if(strcmp(rgb_node.attribute("name").value(),"radiance")) return false;

    std::istringstream s(value);
    s >> _backgroundColor;
    std::cout << "\tCOLOR: " << _backgroundColor;

    return true;
}

bool radiance::io::SceneParser::build(radiance::scene::Scene &scene)
{

    cameras::PerspectiveCamera camera{
        _cameraParams.viewingPlane,
        _cameraParams.fov,
        _cameraParams.lensRadius,
        _cameraParams.lookFrom,
        _cameraParams.lookAt,
        _cameraParams.up
    };

    //Set camera
    scene.setCamera(camera);

    //Set geometry materials
    for(auto& obj: _objects){
        auto id = obj.materialId;
        if(!_materialMap.contains(id)) return false;
        //obj.object->setMaterial(_materialMap[id]);
        auto blue_material = std::make_shared<materials::Diffuse>(math::Color3{0,0,1});
        obj.object->setMaterial(blue_material);
        scene.addObject(obj.object);
        assert(obj.object->_material != nullptr);
    }    


    return true;
}
