#include <array>
#include <cstdint>
#include <iostream>

#include "zobrist.hpp"
#include "types.hpp"

namespace Zobrist {
    // global variables
    std::array<uint64_t, 4> seed;
    std::array<std::array<uint64_t, BOARD_SIZE>, NUM_PIECE_TYPES> pieceKeys;
    std::array<uint64_t, 4> castlingKeys;
    std::array<uint64_t, 8> enPassKeys;
    uint64_t isBlackKey;

    // xoshiro256++
    // https://prng.di.unimi.it/xoshiro256plusplus.c
    uint64_t rand64() {
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

    // zobrist hashing
    void init() {
        // 16 different 16 bit integers were collected from random.org and assembled together to form the 256 bit seed
        // feel free to experiment with different ones
        seed = {0x360BAFF383999633ull, 0x68EE16F8479F8123ull, 0xA89ADF4551B3B25Bull, 0x4887B5A003B21D40ull};
        for (auto& piece: pieceKeys) {
            for (auto& square: piece) {
                square = rand64();
            }
        }
        for (auto& key: castlingKeys) {
            key = rand64();
        }
        for (auto& key: enPassKeys) {
            key = rand64();
        }
        isBlackKey = rand64();
    }
} // namespace Zobrist

