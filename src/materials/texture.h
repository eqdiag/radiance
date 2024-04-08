#pragma once

#include "math/vec.h"
#include "geometry/hit.h"
#include "io/image.h"

namespace radiance{

    namespace materials{

        class Texture{
            public:
                virtual math::Color3 getColor(const geometry::Hit& hit) const = 0;
        };


        class ConstantTexture: public Texture{
            public:
                ConstantTexture(math::Color3 color);
                math::Color3 getColor(const geometry::Hit& hit) const override;

            private:
                math::Color3 color;
        };  

        class ImageTexture: public Texture{
            public:
                ImageTexture(std::shared_ptr<io::Image<math::Color3>> image);
                math::Color3 getColor(const geometry::Hit& hit) const override;
                
            private:
                std::shared_ptr<io::Image<math::Color3>> image{};
        };
    }
}