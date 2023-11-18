#include <iostream>
#include <cstdint>

#include "timeman.hpp"

namespace Timeman {

using namespace std::chrono;

bool TimeManager::hardTimeUp() const {
    auto currTime = high_resolution_clock::now();
    return duration_cast<microseconds>(currTime - startTime).count() > hardTimeLimit;
}

bool TimeManager::softTimeUp() const {
    auto currTime = high_resolution_clock::now();
    return duration_cast<microseconds>(currTime - startTime).count() > softTimeLimit;
}

int64_t TimeManager::getTimeElapsed() const {
    auto currTime = high_resolution_clock::now();
    return duration_cast<milliseconds>(currTime - startTime).count();
}

} // namespace Timeman
