#include "timer.h"
#include <iostream>

void radiance::misc::Timer::setName(const char *name)
{
    _name = name;
}

void radiance::misc::Timer::start()
{
    _start = std::chrono::high_resolution_clock::now();
}

void radiance::misc::Timer::end()
{
    _end = std::chrono::high_resolution_clock::now();
}

void radiance::misc::Timer::displaySeconds() const
{
    float value = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count()) / 1000000.0f;
    if(value > 60.0f){
        std::cout << _name << " took: " << value / 60.0f << " minutes, " << static_cast<float>(fmod(value,60.0f)) << " seconds\n";
    }else{
        std::cout << _name << " took: " << value << " seconds\n";
    }

}