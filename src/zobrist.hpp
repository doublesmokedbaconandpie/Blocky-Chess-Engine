#pragma once

#include <array>
#include <cstdint>

#include "types.hpp"

namespace Zobrist {
    extern std::array<uint64_t, 4> seed;
    static inline uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }
    uint64_t rand64();

    extern std::array<std::array<uint64_t, BOARD_SIZE>, NUM_PIECE_TYPES> pieceKeys;
    extern std::array<uint64_t, 4> castlingKeys;
    extern std::array<uint64_t, 8> enPassKeys;
    extern uint64_t isBlackKey;
    void init();
} // namespace Zobrist