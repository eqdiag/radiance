#pragma once

#include <vector>
#include "math/vec.h"


namespace radiance{

    namespace io{

        //Supported image types (for reads writes)
        enum class ImageType{ PNG, JPG, EXR, UNSUPPORTED};

        ImageType extensionToImageType(const char* ext,size_t len);

        template <typename T>
        class Image{
            public:
                Image():
                    width{0},
                    height{0}
                {
                }

                Image(int width,int height):
                    width{width},
                    height{height}
                {
                    data.resize(width*height);
                }

                void resize(int width,int height){
                    this->width = width;
                    this->height = height;
                    data.resize(width*height);
                }

                void write(int i,int j,T value){
                    data[i*width +j] = value;
                }

                void write(int i,T value){
                    data[i] = value;
                }

                T read(int i,int j) const{
                    return data[i*width+j];
                }

                T read(int i) const{
                    return data[i];
                }

                int width,height;

            private:
                std::vector<T> data{};
        };


        /* Reads */

        bool readRGBImageFromFile(Image<math::Color3>& image,const char* filename);

        bool readRGBImageFromPNG(Image<math::Color3>& image,const char* filename);

        bool readRGBImageFromJPG(Image<math::Color3>& image,const char* filename);

        bool readRGBImageFromEXR(Image<math::Color3>& image,const char* filename);



        /* Writes */

        bool writeRGBImageToPPM(const Image<math::Color3> &image,const char* filename);

        bool writeRGBImageToPNG(const Image<math::Color3>& image,const char* filename);
     
        
       
    }
}