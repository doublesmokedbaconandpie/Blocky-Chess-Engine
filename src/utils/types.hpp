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

constexpr int BOARD_SIZE = 64;
constexpr int NUM_FILES = 8;
constexpr int NUM_RANKS = 8;
constexpr int NUM_BITBOARDS = 14;
constexpr int NUM_COLORED_PIECES = 12;
constexpr int NUM_PIECES = 6;
constexpr int MAX_MOVES = 256;
constexpr int MAX_PLY = 250;

using Square = uint8_t;
using PieceSets = std::array<uint64_t, NUM_BITBOARDS>;
using HistoryTable = std::array<std::array<uint64_t, BOARD_SIZE>, BOARD_SIZE>;
using RNGSeed = std::array<uint64_t, 4>;

inline int getFile(Square square) {
    return square % 8;
}

inline int getRank(Square square) {
    return square / 8;
}