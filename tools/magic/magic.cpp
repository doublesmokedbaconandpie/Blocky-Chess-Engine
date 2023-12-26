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

std::array<uint64_t, 4> seed = {0xf38f4541449b0fc3, 0x8432cf48703f8864, 0x1c8596ae5c1621d1, 0xf6d3be81a796f876};

int main() {
    int attacksSize = 0; 
    std::cout << "Rook attacks: " << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        const uint64_t blockerMask = getRelevantBlockerMask(i, false);
        const uint64_t magic = findMagic(computeRookAttacks, i, blockerMask, blockerRookCombos[i]);
        attacksSize += c_u64(1) << blockerRookCombos[i];
        printHex(magic);
    }
    std::cout << "Rook attacks size: " << attacksSize << std::endl;
    std::cout << std::endl;

    attacksSize = 0;
    std::cout << "Bishop attacks: " << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        const uint64_t blockerMask = getRelevantBlockerMask(i, true);
        const uint64_t magic = findMagic(computeBishopAttacks, i, blockerMask, blockerBishopCombos[i]);
        attacksSize += c_u64(1) << blockerBishopCombos[i];
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
        const int maxIndex = c_u64(1) << shift;
        // magic numbers with low number of 1s are better
        magic = Zobrist::rand64(seed) & Zobrist::rand64(seed) & Zobrist::rand64(seed);
        
        // checks for collisions
        for (uint64_t blocker: possibleBlockers) {
            const size_t index = blocker * magic >> (64 - shift); // hash function
            if (index > maxIndex) {
                magicFound = false;
                break;
            } 
            
            const uint64_t validAttacks = slidingAttacks(square, blocker);
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
