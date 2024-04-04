#pragma once

#define LOG(x) std::cout << "LOG: " << x << "\n";
#define WARN(x) std::cout << "WARNING: " << x << "\n"; abort();
#define ERR(x) std::cout << "ERROR: " << x << "\n"; abort();
