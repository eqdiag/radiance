#include "timer.h"
#include <iostream>

void radiance::misc::Timer::start()
{
    _start = std::chrono::system_clock::now();
}

void radiance::misc::Timer::end()
{
    _end = std::chrono::system_clock::now();
}

long long radiance::misc::Timer::getSeconds() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(_end - _start).count();
}

void radiance::misc::Timer::displaySeconds() const
{
    std::cout << _name << " took: " << getSeconds() << " seconds\n";
}
long long radiance::misc::Timer::getMilliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
}

void radiance::misc::Timer::displayMilliseconds() const
{
    std::cout << _name << " took: " << getMilliseconds() << " milliseconds\n";
}
