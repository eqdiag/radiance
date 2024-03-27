#pragma once

#include "scene/scene.h"
#include "geometry/hit.h"
#include "materials/material.h"
#include "cameras/perspective_camera.h"

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
                bool parseSceneSensorNode(radiance::scene::Scene& scene,pugi::xml_node& sensor_node);
                bool parseSceneMaterialNode(radiance::scene::Scene& scene,pugi::xml_node& material_node,std::string& materialName);        
                bool parseSceneLightNode(radiance::scene::Scene& scene,pugi::xml_node& light_node);        
                bool parseSceneGeometryNode(radiance::scene::Scene& scene,pugi::xml_node& geometry_node);
                bool parseSceneTransformNode(radiance::scene::Scene& scene,pugi::xml_node& transform_node,math::Transform& transform);    
                bool parseSceneBackgroundNode(radiance::scene::Scene& scene,pugi::xml_node& bg_node);        
    

                bool build(radiance::scene::Scene& scene);

                const char* _fileName{};
                const char* _fileDir{};

                //True = y,False = x
                std::optional<bool> yfov{};

                CameraParams _cameraParams{};

                int _inlineMaterialCounter{0};

                bool _backgroundSet{false};
                math::Color3 _backgroundColor{};

                std::unordered_map<std::string,std::shared_ptr<materials::Material>> _materialMap{};
                std::vector<lights::PointLight> _lights{};
                std::vector<HittableParams> _objects{};

                bool _debugMessages{false};

        };


     
       
    }
}