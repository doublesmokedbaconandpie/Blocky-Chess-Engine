#pragma once

#include <chrono>
#include <cstdint>

namespace Timeman {

constexpr int INF_TIME = 1000000000;

struct TimeManager
{
    // We need this for search.hpp to not die
    TimeManager() {
        startTime = std::chrono::high_resolution_clock::now();
        timeLimit = INF_TIME;
    }

    TimeManager(int ms) {
        startTime = std::chrono::high_resolution_clock::now();
        timeLimit = ms * 1000 / 20;
    }

    bool timeUp() const;
    uint64_t getTimeElapsed() const;
    
    std::chrono::system_clock::time_point startTime; 
    uint64_t timeLimit;
};

} // namespace Timeman
