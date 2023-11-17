#pragma once

#include <chrono>
#include <cstdint>

namespace Timeman {

constexpr uint64_t INF_TIME = 1 << 28;

struct TimeManager
{
    TimeManager(int time = INF_TIME, int inc = 0) {
        // convert into microseconds
        time *= 1000;
        inc *= 1000;

        this->startTime = std::chrono::high_resolution_clock::now();
        this->hardTimeLimit = time / 20 + inc / 2;
        this->softTimeLimit = this->hardTimeLimit / 2;
    }

    bool hardTimeUp() const;
    bool softTimeUp() const;
    int64_t getTimeElapsed() const;
    
    std::chrono::system_clock::time_point startTime; 
    int64_t hardTimeLimit, softTimeLimit;
};

} // namespace Timeman
