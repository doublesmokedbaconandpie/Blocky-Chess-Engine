#pragma once

#include <chrono>
#include <cstdint>

namespace Timeman {

constexpr uint64_t INF_TIME = 1 << 28;

class TimeManager
{
    public:
        TimeManager() : TimeManager(INF_TIME, 0){};
        TimeManager(uint64_t time, uint64_t inc);
        bool hardTimeUp() const;
        bool softTimeUp() const;
        int64_t getTimeElapsed() const;

    private:
        std::chrono::system_clock::time_point startTime;
        int64_t hardTimeLimit, softTimeLimit;
};

} // namespace Timeman
