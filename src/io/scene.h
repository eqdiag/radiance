#pragma once

#include "scene/scene.h"
#include "geometry/hit.h"
#include "materials/material.h"
#include "cameras/perspective_camera.h"
#include "geometry/tri_mesh.h"

#include "pugixml/src/pugixml.hpp"

#include <optional>
#include <unordered_map>
#include <memory>

namespace radiance{

    namespace io{

        struct CameraParams{
            cameras::ViewingPlane viewingPlane;
            float fov;
            float lensRadius;
            math::Vec3 lookFrom;
            math::Vec3 lookAt;
            math::Vec3 up;
        };

        struct MaterialParams{
            std::shared_ptr<materials::Material> material;
            std::string textureId;
        };

        struct HittableParams{
            std::shared_ptr<geometry::Hittable> object;
            std::string materialId;
        };

        class SceneParser{
            public:
                SceneParser();
                bool readSceneFromFile(radiance::scene::Scene& scene,const char* fileName,bool debugMessages = false);  



                cameras::ViewingPlane getViewingPlane() const;
            private:

                //Scene parsing
                bool parseSceneSensorNode(radiance::scene::Scene& scene,pugi::xml_node& sensor_node);

                //Texture parsing
                bool parseSceneTextureNode(radiance::scene::Scene& scene,pugi::xml_node& texture_node,std::string& textureName);
                
                //Material parsing
                bool parseSceneMaterialNode(radiance::scene::Scene& scene,pugi::xml_node& material_node,std::string& materialName);    

                //Light parsing    
                bool parseSceneLightNode(radiance::scene::Scene& scene,pugi::xml_node& light_node);        

                //Geometry parsing
                bool parseSceneGeometryNode(radiance::scene::Scene& scene,pugi::xml_node& geometry_node);

                //Transform parsing
                bool parseSceneTransformNode(radiance::scene::Scene& scene,pugi::xml_node& transform_node,math::Transform& transform);   

                //Background parsing 
                bool parseSceneBackgroundNode(radiance::scene::Scene& scene,pugi::xml_node& bg_node);        
    

                bool build(radiance::scene::Scene& scene);

                const char* _fileName{};
                const char* _fileDir{};

                //True = y,False = x
                std::optional<bool> yfov{};

                CameraParams _cameraParams{};

                int _inlineMaterialCounter{0};
                int _inlineTextureCounter{0};

                bool _backgroundSet{false};
                math::Color3 _backgroundColor{};

                std::unordered_map<std::string,std::shared_ptr<radiance::textures::Texture>> _textureMap{};
                std::unordered_map<std::string,std::shared_ptr<materials::Material>> _materialMap{};
                std::vector<lights::PointLight> _lights{};
                std::vector<MaterialParams> _materials{};
                std::vector<HittableParams> _objects{};


                bool _debugMessages{false};

        };


     
       
    }
}