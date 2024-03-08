#pragma once

#include <memory>
#include <vector>

#include "geometry/sphere.h"
#include "lights/point_light.h"

namespace scene{

    class Scene: public geometry::Hittable{
        public:
            math::Color3 shade(const math::Ray& ray);
            bool trace(const math::Ray& ray,geometry::Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;

            math::Color3 backgroundColor(const math::Ray& ray);

            void addObject(std::shared_ptr<geometry::Hittable> object);
            void addLight(const lights::PointLight& light);
        private:
            std::vector<std::shared_ptr<geometry::Hittable>> _objects{};
            std::vector<lights::PointLight> _lights{};
    };
}