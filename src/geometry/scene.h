#pragma once

#include "hit.h"

#include "math/vec.h"
#include "io/image.h"
#include "cameras/perspective.h"

#include <vector>
#include <memory>

namespace radiance{

    namespace geometry{

        class Scene{    
            public:

                Scene(HitList&& hitList,bool buildBHV = false);

                math::Color3 radiance(const math::Ray& ray,int depth,float bounceOffset);

                void render(io::Image<math::Color3>& image,const cameras::Perspective& camera, int samplesPerPixel,int maxBounces,float bounceOffset = 0.0001);


            private:

                radiance::math::Color3 background(const radiance::math::Ray& ray) const;

                std::shared_ptr<geometry::Hittable> sceneRoot{};
        };
    }
}