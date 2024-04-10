#include "image.h"
#include "math/util.h"

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
#define TINYEXR_IMPLEMENTATION
#include "tiny_exr/tinyexr.h"

radiance::io::ImageType radiance::io::extensionToImageType(const char *ext, size_t len)
{

    char lower_case[len];
    
    for(size_t i = 0;i < len;i++){
        lower_case[i] = tolower(ext[i]);
    }

    if(!strcmp(lower_case,".png")){
        return ImageType::PNG;
    }else if(!strcmp(lower_case,".jpg")){
        return ImageType::JPG;
    }else if(!strcmp(lower_case,".exr")){
        return ImageType::EXR;
    }else{
        return ImageType::UNSUPPORTED;
    }

}


bool radiance::io::readRGBImageFromPNG(Image<math::Color3> &image, const char *filename)
{
    int w,h,num_channels;
    unsigned char* data = stbi_load(filename,&w,&h,&num_channels,3);

    if(!data){
        std::cerr << "ERROR: Failed to load alleged png image " << filename << std::endl;
        return false;
    }

    image.resize(w,h);

    for(int i = 0;i<w*h;i++){
        auto color = math::Color3{
            static_cast<float>(data[3*i]) / 255.99f,
            static_cast<float>(data[3*i+1]) / 255.99f,
            static_cast<float>(data[3*i+2]) / 255.99f
        };
        color = math::gammaToLinear(color,2.0);
        image.write(i,color);

    }

    stbi_image_free(data);

    return true;
}

bool radiance::io::readRGBImageFromJPG(Image<math::Color3> &image, const char *filename)
{
    int w,h,num_channels;
    unsigned char* data = stbi_load(filename,&w,&h,&num_channels,3);

    if(!data){
        std::cerr << "ERROR: Failed to load alleged jpg image " << filename << std::endl;
        return false;
    }

    image.resize(w,h);

    for(int i = 0;i<w*h;i++){
        auto color = math::Color3{
            static_cast<float>(data[3*i]) / 255.99f,
            static_cast<float>(data[3*i+1]) / 255.99f,
            static_cast<float>(data[3*i+2]) / 255.99f
        };
        color = math::gammaToLinear(color,2.0);
        image.write(i,color);
    }


    stbi_image_free(data);

    return true;
}

bool radiance::io::readRGBImageFromEXR(Image<math::Color3> &image, const char *filename)
{
    float * data;
    int width,height;
    int num_channels = 3;
    const char* err;
    if( LoadEXR(&data,&width,&height,filename,&err) < 0){
        std::cout << err << std::endl;
        FreeEXRErrorMessage(err);
        return false;
    }

    image.resize(width,height);


    //Copy data into buffer
    for(int i = 0;i < width*height;i++){
        image.write(i,
        math::Color3{
            data[4*i],data[4*i+1],data[4*i+2]
        });
    }


    return true;
}

bool radiance::io::writeRGBImageToPPM(const Image<math::Color3> &image, const char *filename)
{
    std::ofstream out_file(filename);

    //PPM header
    out_file << "P3\n";
    out_file << image.width << " " << image.height << "\n";
    out_file << 255 << "\n";

    //PPM contents
    for(int i = 0;i < image.height;i++){
        for(int j= 0;j< image.width;j++){
            auto color = radiance::math::linearToGamma(image.read(i,j),2.0);
            color *= 255.99;
            int r = static_cast<int>(color.r());
            int g = static_cast<int>(color.g());
            int b = static_cast<int>(color.b());
            out_file << r << " " << g << " " << b << "\n";

        }
    }

    out_file.close();

    return true;
}

bool radiance::io::writeRGBImageToPNG(const Image<math::Color3> &image, const char *filename)
{
    int channels = 3;
    int width = image.width;
    int height = image.height;
    int stride = channels*image.width*sizeof(char);

    char* data = new char[width*height*3];
    for(int i = 0;i< width*height;i++){
        auto col = radiance::math::linearToGamma(image.read(i),2.0);
        col = col.clamp(0.0,1.0);
        col *= 255.9;
        data[3*i] = col.r();
        data[3*i+1] = col.g();
        data[3*i+2] = col.b();
    }

    bool err = stbi_write_png(filename,width,height,channels,(void*)data,stride);
    free(data);
    return err;
}
