#pragma once

#include <array>
#include <algorithm>
#include <cstdint>

enum {
    ALL_SQUARES = 0xFFFFFFFFFFFFFFFFull,

    RANK_1 = 0xFF00000000000000ull,
    RANK_2 = 0x00FF000000000000ull,
    RANK_3 = 0x0000FF0000000000ull,
    RANK_4 = 0x000000FF00000000ull,
    RANK_5 = 0x00000000FF000000ull,
    RANK_6 = 0x0000000000FF0000ull,
    RANK_7 = 0x000000000000FF00ull,
    RANK_8 = 0x00000000000000FFull,

    FILE_A = 0x0101010101010101ull,
    FILE_B = 0x0202020202020202ull,
    FILE_C = 0x0404040404040404ull,
    FILE_D = 0x0808080808080808ull,
    FILE_E = 0x1010101010101010ull,
    FILE_F = 0x2020202020202020ull,
    FILE_G = 0x4040404040404040ull,
    FILE_H = 0x8080808080808080ull,

    NOT_FILE_A = FILE_A ^ ALL_SQUARES,
    NOT_FILE_AB = (FILE_A | FILE_B) ^ ALL_SQUARES,
    NOT_FILE_H = FILE_H ^ ALL_SQUARES,
    NOT_FILE_HG = (FILE_H | FILE_G) ^ ALL_SQUARES,

    DIAG_0  = 0x0000000000000001ull,
    DIAG_1  = 0x0000000000000102ull,
    DIAG_2  = 0x0000000000010204ull,
    DIAG_3  = 0x0000000001020408ull,
    DIAG_4  = 0x0000000102040810ull,
    DIAG_5  = 0x0000010204081020ull,
    DIAG_6  = 0x0001020408102040ull,
    DIAG_7  = 0x0102040810204080ull,
    DIAG_8  = 0x0204081020408000ull,
    DIAG_9  = 0x0408102040800000ull,
    DIAG_10 = 0x0810204080000000ull,
    DIAG_11 = 0x1020408000000000ull,
    DIAG_12 = 0x2040800000000000ull,
    DIAG_13 = 0x4080000000000000ull,
    DIAG_14 = 0x8000000000000000ull,
};

const static std::array<uint64_t, 8> FILES_MASK = {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};
const static std::array<uint64_t, 8> RANKS_MASK = {RANK_8, RANK_7, RANK_6, RANK_5, RANK_4, RANK_3, RANK_2, RANK_1};
const static std::array<uint64_t, 15> DIAGS_MASK = {DIAG_0, DIAG_1, DIAG_2, DIAG_3, DIAG_4, DIAG_5, DIAG_6, DIAG_7,
                                                 DIAG_8, DIAG_9, DIAG_10, DIAG_11, DIAG_12, DIAG_13, DIAG_14};

int leadingBit(uint64_t bitboard);
int trailingBit(uint64_t bitboard);
uint64_t flipVertical(uint64_t bitboard);

int getFile(int square);
int getRank(int square);

uint64_t getFileMask(int square);
uint64_t getRankMask(int square);
uint64_t getDiagMask(int square);
uint64_t getAntiDiagMask(int square);

bool diagAttackers(int square, uint64_t allPieces, uint64_t enemies);
bool straightAttackers(int square, uint64_t allPieces, uint64_t enemies);
bool knightAttackers(int square, uint64_t enemyKnights);
bool pawnAttackers(int square, uint64_t enemyPawns, bool isWhiteTurn);
bool kingAttackers(int square, uint64_t enemyKings);

void printBitboard(uint64_t bitboard);