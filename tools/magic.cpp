// this tool is used to generate magic bitboards for rook and bishop movement hashing

#include <array>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

#include "magic.hpp"
#include "bitboard.hpp"
#include "zobrist.hpp" // for rand64()
#include "types.hpp"

int main() {
    Zobrist::init(); // this gives a seed to rand64()

    int attacksSize = 0;
    std::cout << "Rook attacks: " << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        uint64_t magic = findRookMagic(i, blockerRookCombos[i]);
        attacksSize += 1ull << blockerRookCombos[i];
        printHex(magic);
    }
    std::cout << "Rook attacks size: " << attacksSize << std::endl;
    std::cout << std::endl;

    attacksSize = 0;
    std::cout << "Bishop attacks: " << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        uint64_t magic = findBishopMagic(i, blockerBishopCombos[i]);
        attacksSize += 1ull << blockerBishopCombos[i];
        printHex(magic);
    }
    std::cout << "Bishop attacks size: " << attacksSize << std::endl;

    return 0;
}

uint64_t findRookMagic(int square, int offset) {
    // rooks can only attack on their file and rank
    uint64_t rookMask = getRankMask(square) | getFileMask(square);
    rookMask &= getBlockMask(square); 
    std::vector<uint64_t> possibleBlockers = getPossibleBlockers(square, rookMask);
    uint64_t magic = 0;
    
    while (true) {
        std::array<uint64_t, 4096> rookMoves{};
        bool magicFound = true;
        // magic numbers with low number of 1s are better
        magic = Zobrist::rand64() & Zobrist::rand64() & Zobrist::rand64();
        
        // checks for collisions
        for (uint64_t blocker: possibleBlockers) {
            size_t index = blocker * magic >> (64 - offset); // magic function
            uint64_t attack = rookSlidingAttacks(square, blocker);

            if (index > 1ull << offset) {
                magicFound = false;
                break;
            } 
            if (rookMoves[index] == 0) {
                rookMoves[index] = attack;
            } else 
            if (rookMoves[index] != attack) {
                magicFound = false;
                break;
            }
        }
        if (magicFound) {
            break;
        }
    }
    return magic;
}

uint64_t rookSlidingAttacks(int square, uint64_t blockers) {
    uint64_t attacks = 0ull;
    attacks |= fillInDir(square, blockers, 0, 1);
    attacks |= fillInDir(square, blockers, 0, -1);
    attacks |= fillInDir(square, blockers, 1, 0);
    attacks |= fillInDir(square, blockers, -1, 0);
    return attacks;
}

uint64_t findBishopMagic(int square, int offset) {
    // bishops can only attack on their diagonals
    uint64_t bishopMask = getDiagMask(square) | getAntiDiagMask(square);
    bishopMask &= getBlockMask(square); 
    std::vector<uint64_t> possibleBlockers = getPossibleBlockers(square, bishopMask);
    uint64_t magic = 0;
    
    while (true) {
        std::array<uint64_t, 512> bishopMoves{};
        bool magicFound = true;
        // magic numbers with low number of 1s are better
        magic = Zobrist::rand64() & Zobrist::rand64() & Zobrist::rand64();
        
        // checks for collisions
        for (uint64_t blocker: possibleBlockers) {
            size_t index = blocker * magic >> (64 - offset); // magic function
            uint64_t attack = bishopSlidingAttacks(square, blocker);
            
            if (index > 1ull << offset) {
                magicFound = false;
                break;
            } 
            if (bishopMoves[index] == 0) {
                bishopMoves[index] = attack;
            } else 
            if (bishopMoves[index] != attack) {
                magicFound = false;
                break;
            }
        }
        if (magicFound) {
            break;
        }
    }
    return magic;
}

uint64_t bishopSlidingAttacks(int square, uint64_t blockers) {
    uint64_t attacks = 0ull;
    attacks |= fillInDir(square, blockers, 1, 1);
    attacks |= fillInDir(square, blockers, 1, -1);
    attacks |= fillInDir(square, blockers, 1, -1);
    attacks |= fillInDir(square, blockers, -1, -1);
    return attacks;
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

// Pieces on opposite edges are not blockers, so those do not need to be accounted for
uint64_t getBlockMask(int square) {
    uint64_t validBlockers = ~ALL_EDGES;
    // pieces on the edges are blocked by same edge pieces
    validBlockers |= square / 8 == 0 ? RANK_8 : 0;
    validBlockers |= square / 8 == 7 ? RANK_1 : 0;
    validBlockers |= square % 8 == 0 ? FILE_A : 0;
    validBlockers |= square % 8 == 7 ? FILE_H : 0;
    // the current square and corners are never valid blockers
    validBlockers &= ~(1ull << square);
    validBlockers &= 0x7EFFFFFFFFFFFF7Eull;
    return validBlockers;
}

