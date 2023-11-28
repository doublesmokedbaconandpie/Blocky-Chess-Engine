#include <iostream>
#include <cstdint>

#include "timeman.hpp"

namespace Timeman {

using namespace std::chrono;

TimeManager::TimeManager(uint64_t time, uint64_t inc) {
    // convert into microseconds
    time *= 1000;
    inc *= 1000;

    this->startTime = std::chrono::high_resolution_clock::now();
    this->hardTimeLimit = time / 20 + inc / 2;
    this->softTimeLimit = this->hardTimeLimit / 2;
}

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
