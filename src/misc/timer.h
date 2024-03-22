#pragma once

#include <chrono>

namespace radiance{

    namespace misc{

        class Timer{
            public:
                Timer(const char* name):_name{name}{}
                void start();
                void end();

                long long getSeconds() const;
                void displaySeconds() const;

                long long getMilliseconds() const;
                void displayMilliseconds() const;
            private:
                const char* _name{"none"};
                std::chrono::system_clock::time_point _start;
                std::chrono::system_clock::time_point _end;

        };
    }
}