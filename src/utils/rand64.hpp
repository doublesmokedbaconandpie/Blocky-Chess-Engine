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

#include "types.hpp"

constexpr uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}
// xoshiro256++
// https://prng.di.unimi.it/xoshiro256plusplus.c
constexpr uint64_t rand64(RNGSeed& seed) {
    const uint64_t result = rotl(seed[0] + seed[3], 23) + seed[0];
    const uint64_t t = seed[1] << 17;
    seed[2] ^= seed[0];
    seed[3] ^= seed[1];
    seed[1] ^= seed[2];
    seed[0] ^= seed[3];
    seed[2] ^= t;
    seed[3] = rotl(seed[3], 45);
    return result;
}
