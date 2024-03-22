#include "scene.h"
#include <sstream>
#include <string>

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
    _cameraParams.lookFrom = math::Vec3{0,0,0};
    _cameraParams.lookAt = math::Vec3{0,0,-1};
    _cameraParams.up = math::Vec3{0,1,0};
}

bool radiance::io::SceneParser::readSceneFromFile(radiance::scene::Scene &scene, const char *fileName,bool debugMessages)
{
    _debugMessages = debugMessages;

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
                std::string dummy{};
                if(!parseSceneMaterialNode(scene,child,dummy)) return false;
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

cameras::ViewingPlane radiance::io::SceneParser::getViewingPlane() const
{
    return _cameraParams.viewingPlane;
}

bool radiance::io::SceneParser::parseSceneSensorNode(radiance::scene::Scene &scene, pugi::xml_node &sensor_node)
{
    if(_debugMessages) std::cout << "PARSING SENSOR\n";

    auto cam_type = sensor_node.attribute("type").value();

    //Only support perspective cameras right now
    if(strcmp(cam_type,"perspective")) return false;


    for(auto& child: sensor_node.children()){

        //Parse fov type
        if(!strcmp(child.name(),"string")){
            auto name = child.attribute("name").value();
            auto value = child.attribute("value").value();
            if(_debugMessages) std::cout << "\tPARSING FOV TYPE\n";
            if(!strcmp(name,"fovAxis")){
                if(!strcmp(value,"y")){
                    yfov = true;
                }else if(!strcmp(value,"x")){
                    yfov = false;
                    if(_debugMessages) std::cout << "PARSE ERR: Don't currently support different non y-axis FOV!\n";
                    return false;
                }else{
                    return false;
                }
            }
        }

        //Parse location info
        if(!strcmp(child.name(),"transform")){
            if(_debugMessages) std::cout << "\tPARSING CAM INFO\n";
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

            if(_debugMessages) std::cout << "\t\t FROM: " << _cameraParams.lookFrom;
            if(_debugMessages) std::cout << "\t\t AT: " << _cameraParams.lookAt;
            if(_debugMessages) std::cout << "\t\t UP: " << _cameraParams.up;

        }

        //Parse fov value
        if(!strcmp(child.name(),"float")){
            if(_debugMessages) std::cout << "\tPARSING FOV DATA\n";

            auto name = child.attribute("name").value();
            auto value = child.attribute("value").value();

            if(!strcmp(name,"fov")){
                _cameraParams.fov = std::stof(value);
                if(_debugMessages) std::cout << "\t\t FOV: " << _cameraParams.fov << std::endl;
            }

        }
    
        //Parse sampler
        if(!strcmp(child.name(),"sampler")){
            if(_debugMessages) std::cout << "\tPARSING SAMPLER DATA\n";
            auto sampler_type = child.attribute("type").value();
            //Only supports independent sampler rn
            if(strcmp(sampler_type,"independent")){
                if(_debugMessages) std::cout << "PARSE ERR: Don't currently support samplers other than independent!\n";
                return false;
            }
            
            auto sampler_data = child.child("integer");
            auto name = sampler_data.attribute("name").value();
            auto value = sampler_data.attribute("value").value();

            if(!strcmp(name,"sampleCount")){
                _cameraParams.viewingPlane.spp = std::stoi(value);
                if(_debugMessages) std::cout << "\t\tSPP: " << _cameraParams.viewingPlane.spp << std::endl;
            }

        }

        //Parse image info
        if(!strcmp(child.name(),"film")){
            if(_debugMessages) std::cout << "\tPARSING IMAGE DATA\n";

            auto film_type = child.attribute("type").value();

            if(!strcmp(film_type,"hdrfilm")){
                if(_debugMessages) std::cout << "PARSE SOFT ERR: Don't actually support HDR rn but accept width/height info for PNG images!\n";
            }

            for(auto& sub_child: child.children()){

                auto dimension_name = sub_child.attribute("name").value();
                auto dimension_value = sub_child.attribute("value").value();

                if(!strcmp(dimension_name,"width")){
                    _cameraParams.viewingPlane.imageWidth = std::stoi(dimension_value);
                    if(_debugMessages) std::cout << "\t\tWIDTH: " << _cameraParams.viewingPlane.imageWidth << std::endl;
                    
                }else if(!strcmp(dimension_name,"height")){
                    _cameraParams.viewingPlane.imageHeight = std::stoi(dimension_value);
                    if(_debugMessages) std::cout << "\t\tHEIGHT: " << _cameraParams.viewingPlane.imageHeight << std::endl;
                }else{
                    return false;
                }

            }


        }

    }



    return true;
}

bool radiance::io::SceneParser::parseSceneMaterialNode(radiance::scene::Scene &scene, pugi::xml_node &material_node,std::string& materialName)
{
    if(_debugMessages) std::cout << "PARSING MATERIAL\n";

    auto material_type = material_node.attribute("type").value();

    if(material_node.attribute("id").empty()){

        std::string inline_str{"inline_material_"};
        inline_str += std::to_string(_inlineMaterialCounter);
        _inlineMaterialCounter++;
        materialName = inline_str.c_str();
        
    }else{
        materialName = material_node.attribute("id").value();
    }


    if(_materialMap.contains(materialName)){
        if(_debugMessages) std::cout << "PARSE SOFT ERR: Duplicate material id ["<< materialName <<  "] found!\n";
    }

    if(_debugMessages) std::cout <<"\tID: " << materialName << std::endl;

    if(!strcmp(material_type,"diffuse")){


        auto diffuse_data = material_node.child("rgb");
        auto name = diffuse_data.attribute("name").value();
        auto value = diffuse_data.attribute("value").value();
        if(strcmp(name,"reflectance")){ 
            if(_debugMessages) std::cout << "PARSE ERR: Must have reflectance `name` on `rgb` node to be a valid scene file!\n";
            return false;
        }

        math::Color3 albedo{};
        std::istringstream s(value);
        s >> albedo;
        if(_debugMessages) std::cout << "\tPARSING DIFFUSE: " << albedo;

       _materialMap[materialName] = std::make_shared<materials::Diffuse>(albedo);

    }else if(!strcmp(material_type,"mirror")){

        auto mirror_data = material_node.child("rgb");
        auto name = mirror_data.attribute("name").value();
        auto value = mirror_data.attribute("value").value();
        if(strcmp(name,"reflectance")){ 
            if(_debugMessages) std::cout << "PARSE ERR: Must have reflectance `name` on `rgb` node to be a valid scene file!\n";
            return false;
        }

        math::Color3 reflectance{};
        std::istringstream s(value);
        s >> reflectance;
        if(_debugMessages) std::cout << "\tPARSING MIRROR: " << reflectance;
       _materialMap[materialName] = std::make_shared<materials::Mirror>(reflectance);

    }else{
        std::cerr << "PARSE ERR: Don't currently support material: " << material_type <<  "!\n";
        return false;
    }

    return true;
}

bool radiance::io::SceneParser::parseSceneLightNode(radiance::scene::Scene &scene, pugi::xml_node &light_node)
{
    if(_debugMessages) std::cout << "PARSING LIGHT\n";

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

        if(_debugMessages) std::cout << "\tPARSING POINT LIGHT:\n";
        if(_debugMessages) std::cout << "\t\tINTENSITY: " << light.intensity;
        if(_debugMessages) std::cout << "\t\tPOSITION: " << light.position;


    }else{
        if(_debugMessages) std::cerr << "PARSE ERR: Don't currently support light type: " << light_type <<  "!\n";
        return false;
    }

    return true;
}

bool radiance::io::SceneParser::parseSceneGeometryNode(radiance::scene::Scene &scene, pugi::xml_node &geometry_node)
{
    if(_debugMessages) std::cout << "PARSING GEOMETRY\n";

    auto shape_type = geometry_node.attribute("type").value();

    //Parse obj
    if(!strcmp(shape_type,"obj")){
        if(_debugMessages) std::cout << "\tPARSING OBJ\n";


    //Parse ply
    }else if(!strcmp(shape_type,"ply")){
        if(_debugMessages) std::cout << "\tPARSING PLY\n";


    //Parse sphere
    }else if(!strcmp(shape_type,"sphere")){
        auto point_node = geometry_node.child("point");
        auto radius_node = geometry_node.child("float");

        std::string material_name;

        if(geometry_node.child("ref").empty()){
            //std::cout << "PARSE ERR: Don't support inline materials yet!\n";
            if(_debugMessages) std::cout << "\tPARSING INLINE MATERIAL\n";

            auto material_node = geometry_node.child("bsdf");
            if(material_node.empty()) return false;
            
            if(!parseSceneMaterialNode(scene,material_node,material_name)) return false;
            

        }else{
            auto id_node = geometry_node.child("ref");
            material_name = id_node.attribute("id").value();
        }

        if(strcmp(point_node.attribute("name").value(),"center")) return false;

        if(strcmp(radius_node.attribute("name").value(),"radius")) return false;

    

        float x = std::stof(point_node.attribute("x").value());
        float y = std::stof(point_node.attribute("y").value());
        float z = std::stof(point_node.attribute("z").value());

        float r = std::stof(radius_node.attribute("value").value());

        
        math::Vec3 center{x,y,z};
        auto sphere = std::make_shared<geometry::Sphere>(center,r);
        _objects.emplace_back(HittableParams{sphere,material_name});
        if(_debugMessages) std::cout << "\tPARSING SPHERE:\n";
        if(_debugMessages) std::cout << "\t\tCENTER: " << center;
        if(_debugMessages) std::cout << "\t\tRADIUS: " << r << std::endl;
        if(_debugMessages) std::cout <<"\t\tMATERIAL ID: " << material_name << std::endl;


    //Parse rect
    }else if(!strcmp(shape_type,"rect")){
        if(_debugMessages) std::cout << "\tPARSING RECT\n";

    }else{

        if(_debugMessages) std::cerr << "PARSE ERR: Don't currently support shape: " << shape_type <<  "!\n";
        return false;
    }


    return true;
}

bool radiance::io::SceneParser::parseSceneBackgroundNode(radiance::scene::Scene &scene, pugi::xml_node &bg_node)
{
    if(_debugMessages) std::cout << "PARSING BACKGROUND\n";

    auto rgb_node = bg_node.child("rgb");
    auto value = rgb_node.attribute("value").value();

    if(strcmp(rgb_node.attribute("name").value(),"radiance")) return false;

    std::istringstream s(value);
    s >> _backgroundColor;
    if(_debugMessages) std::cout << "\tCOLOR: " << _backgroundColor;

    return true;
}

bool radiance::io::SceneParser::build(radiance::scene::Scene &scene)
{

    int w = _cameraParams.viewingPlane.imageWidth;
    int h = _cameraParams.viewingPlane.imageHeight;
    _cameraParams.viewingPlane.aspectRatio = static_cast<float>(w)/h;

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
        obj.object->setMaterial(_materialMap[id]);
        scene.addObject(obj.object);
    } 

    //Add lights
    for(auto& light: _lights){
        scene.addLight(light);
    }   


    return true;
}
