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

#include <array>
#include <cstdint>

#include "utils/rand64.hpp"

namespace Zobrist {

inline constexpr auto pieceKeys = [] {
    RNGSeed seed = {0x360baff383999633, 0x68ee16f8479f8123, 0xa89adf4551b3b25b, 0x4887b5a003b21d40};
    std::array<std::array<uint64_t, BOARD_SIZE>, NUM_COLORED_PIECES> keyTable{};
    for (auto& piece: keyTable) {
        for (auto& key: piece) {
            key = rand64(seed);
        }
    }
    return keyTable;
}();

inline constexpr auto castlingKeys = [] {
    RNGSeed seed = {0x19b7b1bb3c2d8fe5, 0x8d989b67e947c8f4, 0x2e3d29cda1ecff47, 0x9c1872c99fdb5a3c};
    std::array<uint64_t, 4> keyTable{};
    for (auto& key: keyTable) {
        key = rand64(seed);
    }
    return keyTable;
}();

inline constexpr auto enPassKeys = [] {
    RNGSeed seed = {0x3e9da55e408ecafe, 0x6d569800e894469e, 0x974a657968409560, 0xcb37386a2913c636};
    std::array<uint64_t, 8> keyTable{};
    for (auto& key: keyTable) {
        key = rand64(seed);
    }
    return keyTable;
}();

inline constexpr auto isBlackKey = [] {
    RNGSeed seed = {0x078279f6872cb95e, 0xd675a7617f43a9b5, 0x55c511df8850988f, 0x22789bd6bcf41f28};
    return rand64(seed);
}();

} // namespace Zobrist