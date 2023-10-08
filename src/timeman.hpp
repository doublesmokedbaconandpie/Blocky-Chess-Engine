#pragma once

#include <chrono>
#include <cstdint>

namespace Timeman {
    const int INF_TIME = 1000000000;

    struct TimeManager
    {
        std::chrono::system_clock::time_point startTime; 
        uint64_t timeLimit;

        // We need this for search.hpp to not die
        TimeManager() {
            startTime = std::chrono::high_resolution_clock::now();
            timeLimit = INF_TIME;
        }

        TimeManager(int ms) {
            startTime = std::chrono::high_resolution_clock::now();
            timeLimit = ms * 100;
        }

        bool timeUp() const;
        
        uint64_t getTimeElapsed() const;
    };
    
} // namespace Timeman
