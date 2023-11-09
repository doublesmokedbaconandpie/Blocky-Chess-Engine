#pragma once

#include <chrono>
#include <cstdint>

namespace Timeman {

constexpr int INF_TIME = 1000000000;

struct TimeManager
{
    TimeManager(int ms = INF_TIME) {
        this->startTime = std::chrono::high_resolution_clock::now();
        this->timeLimit = ms * 1000 / 20;
    }

    bool timeUp() const;
    int64_t getTimeElapsed() const;
    
    std::chrono::system_clock::time_point startTime; 
    int64_t timeLimit;
};

} // namespace Timeman
