/*
* Blocky, a UCI chess engine
* Copyright (C) 2023-2024, Kevin Nguyen
*
* Blocky is free software; you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
*
* Blocky is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along with this program; 
* if not, see <https://www.gnu.org/licenses>.
*/

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
