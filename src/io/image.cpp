#include "image.h"

#include <fstream>


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
        
            auto color = image.read(i,j) * 255.99;
            int r = static_cast<int>(color.r());
            int g = static_cast<int>(color.g());
            int b = static_cast<int>(color.b());
            out_file << r << " " << g << " " << b << "\n";

        }
    }

    out_file.close();

    return true;
}
