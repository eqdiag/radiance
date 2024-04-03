#include "scene.h"
#include <sstream>
#include <string>
#include <filesystem>

#include "materials/diffuse.h"
#include "materials/mirror.h"
#include "materials/plastic.h"

#include "io/mesh.h"

#include "math/util.h"

#include "geometry/hit.h"
#include "geometry/tri.h"

#include "misc/timer.h"

#include "acceleration/bvh.h"

#include <cassert>

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
    //Keep file name and directory around
    _fileName = fileName;
    std::filesystem::path full(fileName);
    full.remove_filename();
    _fileDir = full.c_str();

    _debugMessages = debugMessages;

    pugi::xml_document doc{};
    if(doc.load_file(fileName)){
        auto scene_node = doc.child("scene");   

        for(auto& child: scene_node.children()){
            
            //Sensor node
            if(!strcmp(child.name(),"sensor")){
                if(!parseSceneSensorNode(scene,child)) return false;
            }

            //Parse texture node
            if(!strcmp(child.name(),"texture")){
                std::string dummy{};
                if(!parseSceneTextureNode(scene,child,dummy)) return false;
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

    auto cam_type = sensor_node.attribute("type");

    //Only support perspective cameras right now
    if(!cam_type ||  strcmp(cam_type.value(),"perspective")) return false;


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

bool radiance::io::SceneParser::parseSceneTextureNode(radiance::scene::Scene &scene, pugi::xml_node &texture_node, std::string &textureName)
{
    if(_debugMessages) std::cout << "PARSING TEXTURE\n";

    auto type = texture_node.attribute("type");

    if(!type || strcmp(type.value(),"bitmap")){
        std::cerr << "PARSE ERROR: Only support bitmap textures!\n"; 
        return false;
    }

    auto id = texture_node.attribute("id");
    auto name = texture_node.attribute("name");



    if(!id && !name){
        std::cerr << "PARSE ERROR: No texture id or name specified!\n"; 
        return false;
    }

    bool inline_texture = false;

    if(name){
        inline_texture = true;
        if(_debugMessages) std::cout << "\tTEXTURE IS INLINE\n";
    }

    auto path_node = texture_node.child("string");
    if(!path_node){
        std::cerr << "PARSE ERROR: No path to texture specified!\n"; 
        return false;
    }

    auto fname = path_node.attribute("name");

    if(!fname || strcmp(fname.value(),"filename")){
        std::cerr << "PARSE ERROR: Must specify filename for texture!\n"; 
        return false;
    }

    auto value = path_node.attribute("value");

    if(!value){
        std::cerr << "PARSE ERROR: Must specify filename for texture!\n"; 
        return false;
    }

    std::string filename = value.value();
    std::string full_path = std::string{_fileDir} + filename;

    auto ext = std::filesystem::path(filename).extension().string();

    auto img_type = io::extensionToImageType(ext.c_str(),ext.length());


    auto image = std::make_shared<Image<math::Color3>>();


    if(!radiance::io::readRGBImageFromFile(*image,img_type,full_path.c_str())){
        std::cerr << "PARSE ERROR: Failed to load image: " << full_path << std::endl; 
        return false;
    }

    if(_debugMessages){
        std::cout << "\tWIDTH: " << image->getWidth() << std::endl;
        std::cout << "\tHEIGHT: " << image->getHeight() << std::endl;
        std::cout << "\tTEXTURE ID: " << id.value() << std::endl;
    }

    if(_debugMessages) std::cout << "SUCCESS: Read file " << full_path << " into a texture!\n";

    math::Vec2 scale{1,1};
    math::Vec2 offset{0,0};

    for(const auto& child: texture_node.children()){
        if(!strcmp(child.name(),"float")){

            if(child.attribute("name") && child.attribute("value")){

                auto name = child.attribute("name").value();
                auto value = child.attribute("value").value();

                if(!strcmp(name,"uscale")){
                    scale[0] = std::stof(value);
                }else if(!strcmp(name,"vscale")){
                    scale[1] = std::stof(value);
                }else if(!strcmp(name,"uoffset")){
                    offset[0] = std::stof(value);
                }else if(!strcmp(name,"voffset")){
                    offset[1] = std::stof(value);
                }else{
                    std::cerr << "PARSE ERR: Unsupported texture sampling option " << name << std::endl;
                }
                
            }
        }
    }

    if(_debugMessages){
        std::cout << "\tSCALE: " << scale;
        std::cout << "\tOFFSET: " << offset;
    }


    auto texture = std::make_shared<radiance::textures::ImageTexture>(image,scale,offset);
    if(inline_texture){
        std::string inline_str{"inline_texture_"};
        inline_str += std::to_string(_inlineTextureCounter);
        _inlineTextureCounter++;
        textureName = inline_str;
        _textureMap[textureName] = texture;
    }else{
        _textureMap[id.value()] = texture;
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

    auto texture_child = material_node.child("texture");
    bool inline_texture = false;
    std::string texture_name{};

    if(texture_child){
        inline_texture = true;
        if(!parseSceneTextureNode(scene,texture_child,texture_name)) return false;
    }


    if(!strcmp(material_type,"diffuse")){


        auto constant_data = material_node.child("rgb");
        auto texture_data = material_node.child("ref");

        
        //if(_debugMessages) std::cout << "PARSE SOFT ERR: Don't support texture parsing yet!!\n";

        if(constant_data){
            auto name = constant_data.attribute("name").value();
            auto value = constant_data.attribute("value").value();

            if(!strcmp(name,"reflectance")){ 
                math::Color3 albedo{};
                std::istringstream s(value);
                s >> albedo;
                if(_debugMessages) std::cout << "\tPARSING DIFFUSE: " << albedo;
                auto material = std::make_shared<materials::Diffuse>(albedo);
                _materialMap[materialName] = material;
                if(inline_texture){
                    _materials.emplace_back(MaterialParams{material,texture_name});
                }
            }else{
                if(_debugMessages) std::cout << "PARSE ERR: Must have reflectance `name` on `rgb` node to be a valid scene file!\n";
                return false;
            }
        }else if(texture_data){

            auto name = texture_data.attribute("name");
            auto tex_id = texture_data.attribute("id");

            if(!name || strcmp(name.value(),"reflectance")){
                if(_debugMessages) std::cout << "PARSE ERR: Texture must have reflectance tag!\n";
                return false;
            }

            if(!tex_id){
                if(_debugMessages) std::cout << "PARSE ERR: Must reference texture id!\n";
                return false;
            }

            auto material = std::make_shared<materials::Diffuse>();
            _materialMap[materialName] = material;
            _materials.emplace_back(MaterialParams{material,tex_id.value()});

            if(_debugMessages) std::cout << "\tTEXTURE ID: " << tex_id.value() << std::endl;

        }else if(inline_texture){
            auto texture = _textureMap[texture_name];
            auto material = std::make_shared<materials::Diffuse>();
            _materialMap[materialName] = material;
            _materials.emplace_back(MaterialParams{material,texture_name});
            if(_debugMessages) std::cout << "\tTEXTURE ID: " << texture_name << std::endl;
        }else{
            return false;
        }

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

    }else if(!strcmp(material_type,"plastic")){

        if(_debugMessages) std::cout << "\tPARSING PLASTIC\n";

        auto constant_data = material_node.child("rgb");
        auto texture_data = material_node.child("ref");

        if(constant_data){

            if(_debugMessages){
                std::cout << "PARSE ERR: Plastic material doesn't support rgb nodes!\n";
                return false;
            }

            auto name = constant_data.attribute("name").value();
            auto value = constant_data.attribute("value").value();

            if(!strcmp(name,"reflectance")){ 
             
                math::Color3 albedo{};
                std::istringstream s(value);
                s >> albedo;
                auto material = std::make_shared<materials::Diffuse>(albedo);
                _materialMap[materialName] = material;
                if(inline_texture){
                    _materials.emplace_back(MaterialParams{material,texture_name});
                }
            }else{
                if(_debugMessages) std::cout << "PARSE ERR: Must have reflectance `name` on `rgb` node to be a valid scene file!\n";
                return false;
            }
        }else if(texture_data){

            //Get index of refraction
            auto float_child = material_node.child("float");
            if(!float_child) return false;
            auto eta_name = float_child.attribute("name");
            if(!eta_name || strcmp(eta_name.value(),"eta")) return false;

            auto value = float_child.attribute("value");
            if(!value) return false;

            float eta = std::stof(value.value());
            if(_debugMessages) std::cout << "\t\tETA: " << eta << std::endl; 



            auto name = texture_data.attribute("name");
            auto tex_id = texture_data.attribute("id");

            if(!name || strcmp(name.value(),"reflectance")){
                if(_debugMessages) std::cout << "PARSE ERR: Texture must have reflectance tag!\n";
                return false;
            }

            if(!tex_id){
                if(_debugMessages) std::cout << "PARSE ERR: Must reference texture id!\n";
                return false;
            }

            auto material = std::make_shared<materials::Plastic>(eta);
            _materialMap[materialName] = material;
            _materials.emplace_back(MaterialParams{material,tex_id.value()});

            if(_debugMessages) std::cout << "\tTEXTURE ID: " << tex_id.value() << std::endl;

        }else if(inline_texture){

            //Get index of refraction
            auto float_child = material_node.child("float");
            if(!float_child) return false;
            auto eta_name = float_child.attribute("name");
            if(!eta_name || strcmp(eta_name.value(),"eta")) return false;

            auto value = float_child.attribute("value");
            if(!value) return false;

            float eta = std::stof(value.value());
            if(_debugMessages) std::cout << "\t\tETA: " << eta << std::endl; 

            auto texture = _textureMap[texture_name];
            auto material = std::make_shared<materials::Plastic>(eta);
            _materialMap[materialName] = material;
            _materials.emplace_back(MaterialParams{material,texture_name});
            if(_debugMessages) std::cout << "\tTEXTURE ID: " << texture_name << std::endl;
        }else{
            return false;
        }

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

    //Parse obj/ply
    if(!strcmp(shape_type,"obj") || !strcmp(shape_type,"ply")){
        if(_debugMessages) std::cout << "\tPARSING MESH\n";

        std::string material_name;

        if(geometry_node.child("ref").empty()){
            if(_debugMessages) std::cout << "\tPARSING INLINE MATERIAL\n";

            auto material_node = geometry_node.child("bsdf");
            if(material_node.empty()) return false;
            
            if(!parseSceneMaterialNode(scene,material_node,material_name)) return false;
            

        }else{
            auto id_node = geometry_node.child("ref");
            material_name = id_node.attribute("id").value();
        }

        auto filename_node = geometry_node.child("string");
        auto name = filename_node.attribute("name").value();

        if(filename_node.empty() || !name){
            if(_debugMessages) std::cerr << "PARSE ERR: No filename node found for obj/ply file!\n";
            return false;
        }

        auto filename = filename_node.attribute("value").value();
        std::string full_path = std::string{_fileDir} + filename;

        
        auto mesh = std::make_shared<geometry::TriMesh>();
        
        if(!strcmp(shape_type,"obj")){
            if(!io::readTriMeshFromObj(*mesh,full_path.c_str())){
                if(_debugMessages) std::cerr << "PARSE ERR: Couldn't read obj file: " << full_path<< "\n";
                return false;
            }
        }else if(!strcmp(shape_type,"ply")){
            if(!io::readTriMeshFromPly(*mesh,full_path.c_str())){
                if(_debugMessages) std::cerr << "PARSE ERR: Couldn't read ply file: " << full_path<< "\n";
                return false;
            }
        }

        if(_debugMessages) std::cout << "SUCCESS: Read file " << full_path << " into a mesh!\n";

        auto transform_node = geometry_node.child("transform");
        if(transform_node){
            math::Transform transform{};
            if(!parseSceneTransformNode(scene,transform_node,transform)) return false;
            
            //if(_debugMessages) std::cerr << "PARSE ERR: BVH instancing...scary: " << full_path<< "\n";
            //return false;

            auto instance = std::make_shared<geometry::InstancedHittable>(mesh,transform);

            _objects.emplace_back(HittableParams{instance,material_name});

        }else{
        

            _objects.emplace_back(HittableParams{mesh,material_name});
        }
        if(_debugMessages) std::cout <<"\tMATERIAL ID: " << material_name << std::endl;




    }else if(!strcmp(shape_type,"sphere")){
        auto point_node = geometry_node.child("point");
        auto radius_node = geometry_node.child("float");

        std::string material_name;

        if(geometry_node.child("ref").empty()){
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
    }else if(!strcmp(shape_type,"rectangle")){
        if(_debugMessages) std::cout << "\tPARSING RECT\n";

        std::string material_name;

        if(geometry_node.child("ref").empty()){
            if(_debugMessages) std::cout << "\tPARSING INLINE MATERIAL\n";

            auto material_node = geometry_node.child("bsdf");
            if(material_node.empty()) return false;
            
            if(!parseSceneMaterialNode(scene,material_node,material_name)) return false;
            

        }else{
            auto id_node = geometry_node.child("ref");
            material_name = id_node.attribute("id").value();
        }

        std::vector<radiance::geometry::Vertex> vertices = {
            radiance::geometry::Vertex{math::Vec3{-1,-1,0},math::Vec3{0,0,1},math::Vec2{0,0}},
            radiance::geometry::Vertex{math::Vec3{1,-1,0},math::Vec3{0,0,1},math::Vec2{1,0}},
            radiance::geometry::Vertex{math::Vec3{1,1,0},math::Vec3{0,0,1},math::Vec2{1,1}},
            radiance::geometry::Vertex{math::Vec3{-1,1,0},math::Vec3{0,0,1},math::Vec2{0,1}},
        };
        std::vector<uint32_t> indices = {
            0,1,2,
            0,2,3
        };

        auto mesh = std::make_shared<geometry::TriMesh>(std::move(vertices),std::move(indices));


        auto transform_node = geometry_node.child("transform");
        if(transform_node){
            math::Transform transform{};
            if(!parseSceneTransformNode(scene,transform_node,transform)) return false;
            
            //if(_debugMessages) std::cerr << "PARSE ERR: BVH instancing...scary: " << full_path<< "\n";
            //return false;

            auto instance = std::make_shared<geometry::InstancedHittable>(mesh,transform);

            _objects.emplace_back(HittableParams{instance,material_name});

        }else{
        
            _objects.emplace_back(HittableParams{mesh,material_name});
        }
        if(_debugMessages) std::cout <<"\t\tMATERIAL ID: " << material_name << std::endl;


    }else{

        if(_debugMessages) std::cerr << "PARSE ERR: Don't currently support shape: " << shape_type <<  "!\n";
        return false;
    }


    return true;
}

bool radiance::io::SceneParser::parseSceneTransformNode(radiance::scene::Scene &scene, pugi::xml_node &transform_node, math::Transform &transform)
{
    if(_debugMessages) std::cout << "PARSING TRANSFORM NODE\n";

    auto name = transform_node.attribute("name").value();

    if(strcmp(name,"toWorld")) return false;

    //auto scale_node = transform_node.child("scale");

    for(const auto& child_node: transform_node.children()){
        auto child_name = child_node.name();

        if(!strcmp(child_name,"scale")){
            float x = 1;
            float y = 1;
            float z = 1;
            auto x_node = child_node.attribute("x");
            if(x_node) x = std::stof(x_node.value());
            
            auto y_node = child_node.attribute("y");
            if(y_node) y = std::stof(y_node.value());

            auto z_node = child_node.attribute("z");
            if(z_node) z = std::stof(z_node.value());

            math::Vec3 scale{x,y,z};
            transform.scale(scale);
            if(_debugMessages) std::cout << "\tSCALE: " << scale << std::endl;
        }else if(!strcmp(child_name,"translate")){

            float x = 0;
            float y = 0;
            float z = 0;
            auto x_node = child_node.attribute("x");
            if(x_node) x = std::stof(x_node.value());
            
            auto y_node = child_node.attribute("y");
            if(y_node) y = std::stof(y_node.value());

            auto z_node = child_node.attribute("z");
            if(z_node) z = std::stof(z_node.value());

            math::Vec3 translate{x,y,z};
            transform.translate(translate);
            if(_debugMessages) std::cout << "\tTRANSLATE: " << translate << std::endl;
        }else if(!strcmp(child_name,"rotate")){

            float x = 0;
            float y = 0;
            float z = 0;
            float angle_degrees = 0;
            auto x_node = child_node.attribute("x");
            if(x_node) x = std::stof(x_node.value());
            
            auto y_node = child_node.attribute("y");
            if(y_node) y = std::stof(y_node.value());

            auto z_node = child_node.attribute("z");
            if(z_node) z = std::stof(z_node.value());

             auto angle_node = child_node.attribute("angle");
            if(angle_node) angle_degrees = std::stof(angle_node.value());
    
            math::Vec3 axis{x,y,z};
            transform.rotate(math::degToRad(angle_degrees),axis);
            if(_debugMessages) std::cout << "\tAXIS: " << axis << "\n\tANGLE: " << angle_degrees << std::endl;
        }
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
    _backgroundSet = true;

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

    //Set material textures
    for(auto& mat: _materials){
        auto id = mat.textureId;
        if(!_textureMap.contains(id)) return false;
        mat.material->setTexture(_textureMap[id]);
    }

    //Set geometry materials
    for(auto& obj: _objects){
        auto id = obj.materialId;
        if(!_materialMap.contains(id)) return false;
        obj.object->setMaterial(_materialMap[id]);

        auto obj_primitives = obj.object->getPrimitives();

        if(obj_primitives.has_value()){
            std::cout << "Building BVH!\n";
            std::cout << "NUM TRIANGLES: " << obj_primitives.value().size() << std::endl;
            misc::Timer timer{"BVH building..."};
            timer.start();        
            auto bvh_node = std::make_shared<acceleration::BVHNode>(obj_primitives.value());
            timer.end();
            timer.displaySeconds();
            scene.addObject(bvh_node);
        }else{
            scene.addObject(obj.object);
        }
    } 

    scene.computeBoundingBox();

    //Add lights
    for(auto& light: _lights){
        scene.addLight(light);
    }   

    //Set background
    if(_backgroundSet){
        scene.setBackgroundColor(_backgroundColor);
    }

    return true;
}
