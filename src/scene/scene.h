#pragma once

#include <memory>
#include <vector>
#include <optional>

#include "geometry/sphere.h"
#include "lights/point_light.h"
#include "cameras/perspective_camera.h"

namespace radiance{

    namespace scene{

        class Scene: public geometry::Hittable{
            public:
                Scene(float dt);
                Scene(cameras::PerspectiveCamera& camera);

                void generateImageBuffer(char* pixels) const;

                math::Color3 radiance(const math::Ray& ray,int depth = 0) const;
                bool trace(const math::Ray& ray,geometry::Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;

                math::Color3 backgroundColor(const math::Ray& ray) const;

                void addObject(std::shared_ptr<geometry::Hittable> object);
                void addLight(const lights::PointLight& light);

                void clearObjects();
                void clearLights();

                void setCamera(cameras::PerspectiveCamera& camera);
                void setBackgroundColor(math::Color3& color);

                int getMaxDepth() const;

            private:



                std::vector<std::shared_ptr<geometry::Hittable>> _objects{};
                std::vector<lights::PointLight> _lights{};
                cameras::PerspectiveCamera _camera;

                std::optional<math::Color3> _backgroundColor{};

                static int _maxDepth;
                const int MAX_DEPTH = 400;

            float _DT{0.01};

        };
    }

}