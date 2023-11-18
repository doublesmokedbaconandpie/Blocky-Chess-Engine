#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>

#include "bitboard.hpp"
#include "types.hpp"

// gets index of the least significant bits
int lsb(uint64_t bitboard) {
    assert(bitboard);
    return __builtin_ctzll(bitboard);
}

// gets index of the most significant bit
int msb(uint64_t bitboard) {
    assert(bitboard);
    return 63 - __builtin_clzll(bitboard);
}

int popLsb(uint64_t& bitboard) {
    int pos = lsb(bitboard);
    bitboard ^= c_u64(1) << pos;
    return pos;
}

int popMsb(uint64_t& bitboard) {
    int pos = msb(bitboard);
    bitboard ^= c_u64(1) << pos;
    return pos;
}

int popcount(uint64_t bitboard) {
    return __builtin_popcountll(bitboard);
}

uint64_t flipVertical(uint64_t bitboard) {
    uint64_t k1 = 0x00FF00FF00FF00FF;
    uint64_t k2 = 0x0000FFFF0000FFFF;
    bitboard = ((bitboard >>  8) & k1) | ((bitboard & k1) <<  8);
    bitboard = ((bitboard >> 16) & k2) | ((bitboard & k2) << 16);
    bitboard = ( bitboard >> 32)       | ( bitboard       << 32);
    return bitboard;
}

int getFile(int square) {
    return square % 8;
}
int getRank(int square) {
    return square / 8;
}

uint64_t getFileMask(int square) {
    return FILES_MASK[getFile(square)];
}

uint64_t getRankMask(int square) {
    return RANKS_MASK[getRank(square)];
}

uint64_t getDiagMask(int square) {
    return DIAGS_MASK[getRank(square) + getFile(square)];
}

uint64_t getAntiDiagMask(int square) {
    return flipVertical(DIAGS_MASK[7 - getRank(square) + getFile(square)]);
}

// below functions are for debugging and testing

void printHex(uint64_t bitboard) {
    std::cout << "0x" << std::setw(16) << std::setfill('0') << std::hex <<  bitboard << "\n" << std::dec;
}

void printBitboard(uint64_t bitboard) {
    std::cout << "------------\n";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            bool bit = c_u64(1) << (8 * i + j) & bitboard;
            std::cout << bit << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "------------\n";
}

uint64_t arrayToBitboardNotEmpty(std::array<pieceTypes, BOARD_SIZE> board) {
    uint64_t result = 0;
    for (size_t i = 0; i < board.size(); i++) {
        if (board.at(i) != EmptyPiece) {
            result |= (c_u64(1) << i);
        }
    }
    return result;
}

uint64_t arrayToBitboardPieceType(std::array<pieceTypes, BOARD_SIZE> board, pieceTypes piece) {
    uint64_t result = 0;
    for (size_t i = 0; i < board.size(); i++) {
        if (board.at(i) == piece) {
            result |= (c_u64(1) << i);
        }
    }
    return result;
}