#pragma once

#include "hit.h"

#include "math/vec.h"
#include "io/image.h"
#include "cameras/perspective.h"

#include <vector>
#include <memory>

namespace radiance{

    namespace geometry{

        class Scene: Hittable{    
            public:

                Scene(HitList&& objects);

                bool trace(const math::Ray& ray,Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const;

                void render(io::Image<math::Color3>& image,const cameras::Perspective& camera, int samplesPerPixel,int maxBounces,float bounceOffset = 0.0001);


            private:

                radiance::math::Color3 background(const radiance::math::Ray& ray) const;


                HitList objects{};
        };
    }
}