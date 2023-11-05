#pragma once

#include <array>
#include <cstdint>

#include "types.hpp"

namespace Zobrist {

constexpr uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}
// xoshiro256++
// https://prng.di.unimi.it/xoshiro256plusplus.c
constexpr uint64_t rand64(std::array<uint64_t, 4>& seed) {
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

constexpr auto pieceKeys = [] {
    std::array<uint64_t, 4> seed = {0x360baff383999633ull, 0x68ee16f8479f8123ull, 0xa89adf4551b3b25bull, 0x4887b5a003b21d40ull};
    std::array<std::array<uint64_t, BOARD_SIZE>, NUM_PIECE_TYPES> keyTable{};
    for (auto& piece: keyTable) {
        for (auto& key: piece) {
            key = rand64(seed);
        }
    }
    return keyTable;
}();

constexpr auto castlingKeys = [] {
    std::array<uint64_t, 4> seed = {0x19b7b1bb3c2d8fe5ull, 0x8d989b67e947c8f4ull, 0x2e3d29cda1ecff47ull, 0x9c1872c99fdb5a3cull};
    std::array<uint64_t, 4> keyTable{};
    for (auto& key: keyTable) {
        key = rand64(seed);
    }
    return keyTable;
}();

constexpr auto enPassKeys = [] {
    std::array<uint64_t, 4> seed = {0x3e9da55e408ecafeull, 0x6d569800e894469eull, 0x974a657968409560ull, 0xcb37386a2913c636ull};
    std::array<uint64_t, 8> keyTable{};
    for (auto& key: keyTable) {
        key = rand64(seed);
    }
    return keyTable;
}();

constexpr auto isBlackKey = []{
    std::array<uint64_t, 4> seed = {0x078279f6872cb95eull, 0xd675a7617f43a9b5ull, 0x55c511df8850988full, 0x22789bd6bcf41f28ull};
    uint64_t key = rand64(seed);
    return key;
}();

} // namespace Zobrist