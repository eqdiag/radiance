#pragma once

#include "vec.h"

namespace radiance{

    namespace math{


        /* Misc geometry */

        constexpr float PI = 3.141592653589793;
        constexpr float TWO_PI = 6.28318530718;

        float degToRad(float deg);
        float radToDeg(float rad);

        //Assumes in, and normal are in different directions: in.dot(normal) < 0 
        //Also assumes both vectors are normalized 
        Vec3 reflect(const Vec3& in,const Vec3& normal);

        //Assumes in, and normal are in different directions: in.dot(normal) < 0 */
        //Also assumes both vectors are normalized */
        Vec3 refract(const Vec3& in,const Vec3& normal,float indexRatio);

        //Schlick approximation for total internal reflection
        float schlick(float cos,float indexRatio);

        /* Misc random */

        void initRandom();

        float randomFloat(float min = 0.0f,float max = 1.0f);

        int randInt(int min,int max);

        /* Uses rejection sampling to get a point in unit disk*/
        /*Note: z coordinate is 0*/
        Vec3 randomUnitDisk();


        /* Timing */

        class Timer{
            public:
                Timer(const char* name):name(name){}
                void setName(const char* name);
                void begin();
                void stop();

                void displaySeconds() const;
            private:
                const char* name{"none"};
                std::chrono::high_resolution_clock::time_point start;
                std::chrono::high_resolution_clock::time_point end;

        };
    }

}