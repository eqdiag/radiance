#include "image.h"


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

bool radiance::io::readEXRImageFromFile(Image<math::Color3> &image, const char *filename)
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
        image.set(i,
        math::Color3{
            data[4*i],data[4*i+1],data[4*i+2]
        });
    }


    return true;
}

bool radiance::io::readJPGImageFromFile(Image<math::Color3> &image, const char *filename)
{

    int w,h,num_channels;
    unsigned char* data = stbi_load(filename,&w,&h,&num_channels,3);

    if(!data || (num_channels != 3)){
        std::cerr << "ERROR: Failed to load alleged jpg image " << filename << std::endl;
        return false;
    }

    image.resize(w,h);

    for(int i = 0;i<w*h;i++){
        image.set(i,math::Color3{
            static_cast<float>(data[3*i]) / 255.99f,
            static_cast<float>(data[3*i+1]) / 255.99f,
            static_cast<float>(data[3*i+2]) / 255.99f
        });

    }


    stbi_image_free(data);

    return true;
}

bool radiance::io::readPNGImageFromFile(Image<math::Color3> &image, const char *filename)
{
   
    int w,h,num_channels;
    unsigned char* data = stbi_load(filename,&w,&h,&num_channels,3);

    if(!data){
        std::cerr << "ERROR: Failed to load alleged png image " << filename << std::endl;
        return false;
    }

    image.resize(w,h);

    for(int i = 0;i<w*h;i++){
        float r = pow(static_cast<float>(data[3*i]) / 255.99f,2.2);
        float g = pow(static_cast<float>(data[3*i + 1]) / 255.99f,2.2);
        float b = pow(static_cast<float>(data[3*i + 2]) / 255.99f,2.2);

        image.set(i,math::Color3{r,g,b});

    }



    stbi_image_free(data);

    return true;
}

bool radiance::io::readRGBImageFromFile(Image<math::Color3> &image, ImageType imageType, const char *filename)
{
    if(imageType == ImageType::EXR){
        return readEXRImageFromFile(image,filename);
    }else if(imageType == ImageType::PNG){
        return readPNGImageFromFile(image,filename);
    }else if(imageType == ImageType::JPG){
        return readJPGImageFromFile(image,filename);
    }else{
        return false;
    }

    return true;
}

bool radiance::io::writeRGBImageToPNG(const Image<math::Color3> &image, const char *filename)
{

    int channels = 3;
    int width = image.getWidth();
    int height = image.getHeight();
    int stride = channels*image.getWidth()*sizeof(char);

    char* data = new char[width*height*3];
    for(int i = 0;i< width*height;i++){
        auto col = image.get(i);
        data[3*i] = col.r();
        data[3*i+1] = col.g();
        data[3*i+2] = col.b();
    }

    bool err = stbi_write_png(filename,width,height,channels,(void*)data,stride);
    free(data);
    return err;
}

bool radiance::io::writeRGBImageToFile(const Image<math::Color3> &image, ImageType imageType, const char *filename)
{
    if(imageType == ImageType::PNG){

        return writeRGBImageToPNG(image,filename);

    }else{
        std::cerr << "ERROR: Unsupported image type " << filename << std::endl; 
        return false;
    }

    return false;
}
