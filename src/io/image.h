#pragma once

#include "geometry/tri_mesh.h"

#include <vector>

namespace radiance{

    namespace io{

        //Supported image types (for reads writes)
        enum class ImageType{ PNG, JPG, EXR, UNSUPPORTED};

        ImageType extensionToImageType(const char* ext,size_t len);

        template <typename T>
        class Image{
            public:
                Image();
                Image(int width,int height);

                int getWidth() const;
                int getHeight() const;

                void resize(int width,int height);

                T get(int i) const{
                    return _data[i];
                }

                T get(int x,int y) const{
                    return _data[y*_height + x];
                }

                void set(int i,T value){
                    _data[i] = value;
                }

                void set(int x,int y,T value){
                    _data[y*_width + x] = value;
                }

               


            private:
                int _width,_height;
                std::vector<T> _data{};
        };

        /* Reads */


        bool readEXRImageFromFile(Image<math::Color3>& image,const char* filename);

        bool readJPGImageFromFile(Image<math::Color3>& image,const char* filename);

        bool readPNGImageFromFile(Image<math::Color3>& image,const char* filename);



        bool readRGBImageFromFile(Image<math::Color3>& image,ImageType imageType, const char* filename);


        /* Writes */


        bool writeRGBImageToPNG(const Image<math::Color3> &image,const char* filename);

        bool writeRGBImageToFile(const Image<math::Color3>& image,ImageType imageType, const char* filename);


        /* Image implementation */

        template <typename T>
        inline Image<T>::Image():
        _width{0},
        _height{0}
        {
        }

        template <typename T>
        inline Image<T>::Image(int width, int height) : _width{width},
                                                        _height{height}
        {
            _data.resize(_width*_height*sizeof(T));
        }

        template <typename T>
        inline int Image<T>::getWidth() const
        {
            return _width;
        }

        template <typename T>
        inline int Image<T>::getHeight() const
        {
            return _height;
        }

        template <typename T>
        inline void Image<T>::resize(int width, int height)
        {
            _width = width;
            _height = height;
            _data.resize(_width*_height*sizeof(T));
        }
        
       
    }
}