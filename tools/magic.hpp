#include <cstdint>
#include <vector>

constexpr int blockerRookCombos[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

constexpr int blockerBishopCombos[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};


uint64_t findRookMagic(int square, int offset);
uint64_t rookSlidingAttacks(int square, uint64_t blockers);

uint64_t findBishopMagic(int square, int offset);
uint64_t bishopSlidingAttacks(int square, uint64_t blockers);

std::vector<uint64_t> getPossibleBlockers(int square, uint64_t slideMask);
uint64_t fillInDir(int square, uint64_t blockers, int x, int y);
uint64_t getBlockMask(int square);
