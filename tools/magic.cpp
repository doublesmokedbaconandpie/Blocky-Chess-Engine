// this tool is used to generate magic bitboards for rook and bishop movement hashing

#include <array>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

#include "magic.hpp"
#include "attacks.hpp"
#include "bitboard.hpp"
#include "zobrist.hpp" // for rand64()
#include "types.hpp"

using namespace Attacks;

int main() {
    Zobrist::seed = {0xf38f4541449b0fc3ull, 0x8432cf48703f8864ull, 0x1c8596ae5c1621d1ull, 0xf6d3be81a796f876ull};

    int attacksSize = 0; 
    std::cout << "Rook attacks: " << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        uint64_t blockerMask = getRelevantBlockerMask(i, false);
        uint64_t magic = findMagic(rookSlidingAttacks, i, blockerMask, blockerRookCombos[i]);
        attacksSize += 1ull << blockerRookCombos[i];
        printHex(magic);
    }
    std::cout << "Rook attacks size: " << attacksSize << std::endl;
    std::cout << std::endl;

    attacksSize = 0;
    std::cout << "Bishop attacks: " << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        uint64_t blockerMask = getRelevantBlockerMask(i, true);
        uint64_t magic = findMagic(bishopSlidingAttacks, i, blockerMask, blockerBishopCombos[i]);
        attacksSize += 1ull << blockerBishopCombos[i];
        printHex(magic);
    }
    std::cout << "Bishop attacks size: " << attacksSize << std::endl;

    return 0;
}

template<typename Function>
uint64_t findMagic(Function slidingAttacks, int square, uint64_t blockerMask, int shift) {
    std::vector<uint64_t> possibleBlockers = getPossibleBlockers(blockerMask);
    std::array<uint64_t, 4096> moves{};
    uint64_t magic = 0;
    bool magicFound = false; 
    while (!magicFound) {
        moves.fill(ALL_SQUARES);
        magicFound = true;
        int maxIndex = 1ull << shift;
        // magic numbers with low number of 1s are better
        magic = Zobrist::rand64() & Zobrist::rand64() & Zobrist::rand64();
        
        // checks for collisions
        for (uint64_t blocker: possibleBlockers) {
            size_t index = blocker * magic >> (64 - shift); // hash function
            if (index > maxIndex) {
                magicFound = false;
                break;
            } 
            
            uint64_t validAttacks = slidingAttacks(square, blocker);
            if (moves[index] != validAttacks) {
                if (moves[index] == ALL_SQUARES) {
                    moves[index] = validAttacks;
                }
                else {
                    magicFound = false;
                    break;
                }
            }
        }
    }
    return magic;
}
