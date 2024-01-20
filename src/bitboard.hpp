#pragma once

#include <array>
#include <algorithm>
#include <cstdint>

#include "types.hpp"

enum SQUARE_BBS: uint64_t {
    ALL_SQUARES = 0xFFFFFFFFFFFFFFFF,
    NO_SQUARES = 0,

    RANK_1 = 0xFF00000000000000,
    RANK_2 = 0x00FF000000000000,
    RANK_3 = 0x0000FF0000000000,
    RANK_4 = 0x000000FF00000000,
    RANK_5 = 0x00000000FF000000,
    RANK_6 = 0x0000000000FF0000,
    RANK_7 = 0x000000000000FF00,
    RANK_8 = 0x00000000000000FF,

    FILE_A = 0x0101010101010101,
    FILE_B = 0x0202020202020202,
    FILE_C = 0x0404040404040404,
    FILE_D = 0x0808080808080808,
    FILE_E = 0x1010101010101010,
    FILE_F = 0x2020202020202020,
    FILE_G = 0x4040404040404040,
    FILE_H = 0x8080808080808080,

    NOT_FILE_A = FILE_A ^ ALL_SQUARES,
    NOT_FILE_AB = (FILE_A | FILE_B) ^ ALL_SQUARES,
    NOT_FILE_H = FILE_H ^ ALL_SQUARES,
    NOT_FILE_HG = (FILE_H | FILE_G) ^ ALL_SQUARES,
    ALL_EDGES = FILE_A | FILE_H | RANK_1 | RANK_8, 

    DIAG_0  = 0x0000000000000001,
    DIAG_1  = 0x0000000000000102,
    DIAG_2  = 0x0000000000010204,
    DIAG_3  = 0x0000000001020408,
    DIAG_4  = 0x0000000102040810,
    DIAG_5  = 0x0000010204081020,
    DIAG_6  = 0x0001020408102040,
    DIAG_7  = 0x0102040810204080,
    DIAG_8  = 0x0204081020408000,
    DIAG_9  = 0x0408102040800000,
    DIAG_10 = 0x0810204080000000,
    DIAG_11 = 0x1020408000000000,
    DIAG_12 = 0x2040800000000000,
    DIAG_13 = 0x4080000000000000,
    DIAG_14 = 0x8000000000000000,
};

template <typename T>
constexpr std::uint64_t c_u64(T&& x) {
    return static_cast<std::uint64_t>(std::forward<T>(x));
}

constexpr std::array<uint64_t, NUM_FILES> FILES_MASK = {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};
constexpr std::array<uint64_t, NUM_RANKS> RANKS_MASK = {RANK_8, RANK_7, RANK_6, RANK_5, RANK_4, RANK_3, RANK_2, RANK_1};
constexpr std::array<uint64_t, 15> DIAGS_MASK = {DIAG_0, DIAG_1, DIAG_2, DIAG_3, DIAG_4, DIAG_5, DIAG_6, DIAG_7,
                                                 DIAG_8, DIAG_9, DIAG_10, DIAG_11, DIAG_12, DIAG_13, DIAG_14};

constexpr auto ADJ_FILES_AND_FILES_MASK = []{
    std::array<uint64_t, NUM_FILES> masks = FILES_MASK;
    // A and H files only have 1 adjacent files
    masks[0] |= FILE_B;
    masks[7] |= FILE_G;

    // other files have left and right neighbors
    for (int i = 1; i < 7; ++i) {
        masks[i] |= FILES_MASK[i - 1] | FILES_MASK[i + 1];
    }

    return masks;
}();

int lsb(uint64_t bitboard);
int msb(uint64_t bitboard);
int popLsb(uint64_t& bitboard);
int popMsb(uint64_t& bitboard);
int popcount(uint64_t bitboard);

uint64_t flipVertical(uint64_t bitboard);

uint64_t getFileMask(int square);
uint64_t getRankMask(int square);
uint64_t getDiagMask(int square);
uint64_t getAntiDiagMask(int square);

// for debugging and testing
void printHex(uint64_t bitboard);
void printBitboard(uint64_t bitboard);
uint64_t arrayToBitboardNotEmpty(std::array<pieceTypes, BOARD_SIZE> board);
uint64_t arrayToBitboardPieceType(std::array<pieceTypes, BOARD_SIZE> board, pieceTypes piece);
