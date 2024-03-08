#include "image.h"

#include "stb_image/stb_image_write.h"

bool radiance::io::writeBufferToRGB_PNG(const char *buffer, int width, int height, const char *filename)
{

    int channels = 3;
    int stride = channels*width*sizeof(char);

    return stbi_write_png(filename,width,height,channels,buffer,stride);
}
