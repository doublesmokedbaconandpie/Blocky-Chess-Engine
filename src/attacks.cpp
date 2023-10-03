
#include <array>
#include <cstdint>
#include <vector>
#include <iostream>

#include "attacks.hpp"
#include "bitboard.hpp"
#include "types.hpp"

namespace Attacks {

// global variables
std::array<Magic, BOARD_SIZE> rookTable;
std::array<Magic, BOARD_SIZE> bishopTable;
std::array<uint64_t, 102400> rookAttacks;
std::array<uint64_t, 5248> bishopAttacks;

// functions
void init() {
    initMagicTable(rookTable, rookMagics, rookAttacks, rookSlidingAttacks, false);
    initMagicTable(bishopTable, bishopMagics, bishopAttacks, bishopSlidingAttacks, true);
}

template <typename Function, size_t SIZE>
void initMagicTable(std::array<Magic, BOARD_SIZE>& table, 
                    const std::array<uint64_t, BOARD_SIZE>& magicTable,
                    std::array<uint64_t, SIZE>& attackTable,
                    Function getAttackMask, bool isBishop) {

    int attacksFilled = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        // magic table
        uint64_t blockerMask = getRelevantBlockerMask(i, isBishop);
        table[i].slideMask = blockerMask;
        table[i].magic = magicTable[i];
        table[i].shift = 64 - popCount(blockerMask);
        table[i].offset = attacksFilled;

        // attack table     
        std::vector<uint64_t> blockers = getPossibleBlockers(i, blockerMask);
        for (uint64_t blocker: blockers) {
            uint64_t attacks = getAttackMask(i, blocker);
            int index = getMagicIndex(table[i], blocker);
            attackTable[index] = attacks;
        }
        attacksFilled += 1 << popCount(blockerMask);   
        std::cout << attacksFilled << std::endl;
    } 
}

int getMagicIndex(Magic& entry, uint64_t blockers) {
    return ((blockers * entry.magic) >> entry.shift) + entry.offset;
}

std::vector<uint64_t> getPossibleBlockers(int square, uint64_t slideMask) {
    std::vector<uint64_t> blockerBoards;
    std::vector<int> blockerSquares;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((1ull << i) & slideMask) {
            blockerSquares.push_back(i);
        }
    }

    // binary iteration will be used to iterate through the possible blockers
    uint64_t numBlockerBoards = 1ull << blockerSquares.size();
    for (uint64_t i = 0; i < numBlockerBoards; i++) {
        uint64_t currBlocker = 0ull;
        // convert binary iter to blocker bitboard
        for (int j = 0; j < blockerSquares.size(); j++) {
            uint64_t currBit = i & (1ull << j) ? 1 : 0; 
            currBlocker |= currBit << blockerSquares[j];
        }
        blockerBoards.push_back(currBlocker);
    }

    return blockerBoards;
}

// Works for bishops and rooks
uint64_t getRelevantBlockerMask(int square, bool isBishop) {
    uint64_t slideMask = isBishop ? getDiagMask(square) | getAntiDiagMask(square) : getFileMask(square) | getRankMask(square); 
    uint64_t validBlockers = ~ALL_EDGES;
    // pieces on the edges are blocked by same edge pieces
    validBlockers |= square / 8 == 0 ? RANK_8 : 0;
    validBlockers |= square / 8 == 7 ? RANK_1 : 0;
    validBlockers |= square % 8 == 0 ? FILE_A : 0;
    validBlockers |= square % 8 == 7 ? FILE_H : 0;
    // the current square and corners are never valid blockers
    validBlockers &= ~(1ull << square);
    validBlockers &= 0x7EFFFFFFFFFFFF7Eull;
    return validBlockers & slideMask;
}

uint64_t rookSlidingAttacks(int square, uint64_t blockers) {
    uint64_t attacks = 0ull;
    attacks |= fillInDir(square, blockers, 0, 1);
    attacks |= fillInDir(square, blockers, 0, -1);
    attacks |= fillInDir(square, blockers, 1, 0);
    attacks |= fillInDir(square, blockers, -1, 0);
    return attacks;
}

uint64_t bishopSlidingAttacks(int square, uint64_t blockers) {
    uint64_t attacks = 0ull;
    attacks |= fillInDir(square, blockers, 1, 1);
    attacks |= fillInDir(square, blockers, 1, -1);
    attacks |= fillInDir(square, blockers, 1, -1);
    attacks |= fillInDir(square, blockers, -1, -1);
    return attacks;
}

uint64_t fillInDir(int square, uint64_t blockers, int x, int y) {
    int currX = square % 8 + x;
    int currY = square / 8 + y;
    uint64_t filled = 0ull;
    while ( !(filled & blockers) && 
        currX >= 0 && currX < 8 &&
        currY >= 0 && currY < 8) {

        square = 8 * currY + currX;
        filled |= 1ull << square;
        currX += x; 
        currY += y;
    }
    return filled;
}

} // namespace Attacks