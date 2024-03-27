#pragma once

#include <chrono>

namespace radiance{

    namespace misc{

        class Timer{
            public:
                Timer(const char* name):_name{name}{}
                void setName(const char* name);
                void start();
                void end();

                void displaySeconds() const;
            private:
                const char* _name{"none"};
                std::chrono::high_resolution_clock::time_point _start;
                std::chrono::high_resolution_clock::time_point _end;

        };
    }
}