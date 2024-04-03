#pragma once

#include "math/vec.h"
#include "io/image.h"

#include <memory>

namespace radiance{

    namespace textures{

        //TODO: Create multiple sampler types (nearest neighb,bilinear,etc)

        class Texture{
            public:
                Texture(math::Vec2 scale = math::Vec2{1,1},math::Vec2 offset = math::Vec2{0,0});

                virtual math::Color3 getValue(float u,float v,const math::Vec3& point) const = 0;

            protected:
                math::Vec2 _scale{1,1};
                math::Vec2 _offset{0,0};
        };


        class ConstantTexture: public Texture{
            public:
                ConstantTexture(math::Color3 albedo,math::Vec2 scale = math::Vec2{1,1},math::Vec2 offset = math::Vec2{0,0});
                math::Color3 getValue(float u,float v,const math::Vec3& point) const override;

            private:
                math::Color3 _albedo;
        };
        
        class ImageTexture: public Texture{
            public:
                ImageTexture(std::shared_ptr<radiance::io::Image<math::Color3>> image,math::Vec2 scale = math::Vec2{1,1},math::Vec2 offset = math::Vec2{0,0});
                math::Color3 getValue(float u,float v,const math::Vec3& point) const override;

            private:
                std::shared_ptr<radiance::io::Image<math::Color3>> _image{};
        };
    }
}