#pragma once

#include <chrono>
#include <cstdint>

namespace Timeman {

constexpr uint64_t INF_TIME = 1 << 28;

struct TimeManager
{
    TimeManager(int ms = INF_TIME) {
        this->startTime = std::chrono::high_resolution_clock::now();
        this->hardTimeLimit = ms * 1000 / 20;
        this->softTimeLimit = this->hardTimeLimit / 3;
    }

    bool hardTimeUp() const;
    bool softTimeUp() const;
    int64_t getTimeElapsed() const;
    
    std::chrono::system_clock::time_point startTime; 
    int64_t hardTimeLimit, softTimeLimit;
};

} // namespace Timeman
