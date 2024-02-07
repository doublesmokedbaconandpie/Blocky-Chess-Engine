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
    this->softTimeLimit = this->hardTimeLimit / 3;
}

bool TimeManager::hardTimeUp() const {
    const auto currTime = high_resolution_clock::now();
    return duration_cast<microseconds>(currTime - startTime).count() > hardTimeLimit;
}

bool TimeManager::softTimeUp() const {
    const auto currTime = high_resolution_clock::now();
    return duration_cast<microseconds>(currTime - startTime).count() > softTimeLimit;
}

int64_t TimeManager::getTimeElapsed() const {
    const auto currTime = high_resolution_clock::now();
    return duration_cast<milliseconds>(currTime - startTime).count();
}

} // namespace Timeman
