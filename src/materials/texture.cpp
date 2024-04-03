#include "texture.h"

radiance::textures::Texture::Texture(math::Vec2 scale, math::Vec2 offset):
_scale{scale},_offset{offset}
{

}

radiance::textures::ImageTexture::ImageTexture(std::shared_ptr<radiance::io::Image<math::Color3>> image,math::Vec2 scale,math::Vec2 offset):
Texture(scale,offset),
_image{image}
{

}

math::Color3 radiance::textures::ImageTexture::getValue(float u, float v, const math::Vec3 &point) const
{
    float u_mod = fmod(_scale.x()*u + _offset.x(),1.0f);
    float v_mod = fmod(_scale.y()*v + _offset.y(),1.0f);


    int x = static_cast<int>(u_mod*_image->getWidth());
    int y = static_cast<int>(v_mod*_image->getHeight());

    return _image->get(x,y);
}

radiance::textures::ConstantTexture::ConstantTexture(math::Color3 albedo,math::Vec2 scale,math::Vec2 offset):
Texture(scale,offset),
_albedo{albedo}
{
}

math::Color3 radiance::textures::ConstantTexture::getValue(float u, float v, const math::Vec3 &point) const
{
    return _albedo;
}


