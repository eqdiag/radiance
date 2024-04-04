#pragma once

#include <vector>
#include "math/vec.h"


namespace radiance{

    namespace io{

        //Supported image types (for reads writes)
        enum class ImageType{ PNG, JPG, EXR, UNSUPPORTED};

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
                    width = width;
                    height = height;
                    data.resize(width*height);
                }

                void write(int i,int j,T value){
                    data[i*width +j] = value;
                }

                T read(int i,int j) const{
                    return data[i*width+j];
                }

                int width,height;

            private:
                std::vector<T> data{};
        };

        /* Reads */


        /* Writes */

        bool writeRGBImageToPPM(const Image<math::Color3> &image,const char* filename);


     
        
       
    }
}