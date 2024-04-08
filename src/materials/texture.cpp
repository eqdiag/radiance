#include "texture.h"

radiance::materials::ConstantTexture::ConstantTexture(math::Color3 color):
    color{color}
{
}


radiance::math::Color3 radiance::materials::ConstantTexture::getColor(const geometry::Hit &hit) const
{
    return color;
}

radiance::materials::ImageTexture::ImageTexture(const io::Image<math::Color3> &&image):
    image{image}
{
}

radiance::math::Color3 radiance::materials::ImageTexture::getColor(const geometry::Hit &hit) const
{
    float u = hit.u;
    float v = hit.v;

    int x = static_cast<int>(u*image.width);
    int y = static_cast<int>(v*image.height);

    return image.read(x,y);
}
