#include "timeman.hpp"
#include <iostream>
#include <cstdint>
namespace Timeman {

bool TimeManager::timeUp() const {
    auto currTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(currTime - startTime).count() > timeLimit;
}

uint64_t TimeManager::getTimeElapsed() const {
    auto currTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(currTime - startTime).count();
}

} // namespace Timeman
