#pragma once

#include <chrono>

namespace radiance{

    namespace misc{

        class Timer{
            public:
                Timer(const char* name):_name{name}{}
                void start();
                void end();
                float getSeconds() const;
                void displaySeconds() const;
            private:
                const char* _name{"none"};
                std::chrono::system_clock::time_point _start;
                std::chrono::system_clock::time_point _end;

        };
    }
}