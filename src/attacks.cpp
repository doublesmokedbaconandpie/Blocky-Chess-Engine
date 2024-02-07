/*
* Blocky, a UCI chess engine
* Copyright (C) 2023-2024, Kevin Nguyen
*
* Blocky is free software; you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
*
* Blocky is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along with this program; 
* if not, see <https://www.gnu.org/licenses>.
*/

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include "attacks.hpp"
#include "bitboard.hpp"
#include "utils/rand64.hpp"
#include "utils/types.hpp"

void Attacks::init() {
    // init rook and bishop tables
    initMagicTable(ROOK_TABLE, ROOK_MAGICS, ROOK_ATTACKS, computeRookAttacks, false);
    initMagicTable(BISHOP_TABLE, BISHOP_MAGICS, BISHOP_ATTACKS, computeBishopAttacks, true);

    // init king, knight, and pawn tables
    for (int i = 0; i < BOARD_SIZE; i++) {
        KNIGHT_ATTACKS[i] = computeKnightAttacks(i);
        KING_ATTACKS[i] = computeKingAttacks(i);
        PAWN_ATTACKS[0][i] = computePawnAttacks(i, true);
        PAWN_ATTACKS[1][i] = computePawnAttacks(i, false);
    }

}

uint64_t Attacks::rookAttacks(int square, uint64_t allPieces) {
    return ROOK_ATTACKS[getMagicIndex(ROOK_TABLE[square], allPieces)];
}

uint64_t Attacks::bishopAttacks(int square, uint64_t allPieces) {
    return BISHOP_ATTACKS[getMagicIndex(BISHOP_TABLE[square], allPieces)];
}

uint64_t Attacks::pawnAttacks(int square, bool isWhiteTurn) {
    const int color = isWhiteTurn ? 0 : 1;
    return PAWN_ATTACKS[color][square];
}

uint64_t Attacks::knightAttacks(int square) {
    return KNIGHT_ATTACKS[square];
}

uint64_t Attacks::kingAttacks(int square) {
    return KING_ATTACKS[square];
}

int Attacks::getMagicIndex(Magic& entry, uint64_t allPieces) {
    const uint64_t blockers = allPieces & entry.slideMask;
    return ((blockers * entry.magic) >> entry.shift) + entry.offset;
}

template <typename Function, size_t SIZE>
void Attacks::initMagicTable(std::array<Magic, BOARD_SIZE>& table,
                    const std::array<uint64_t, BOARD_SIZE>& magicTable,
                    std::array<uint64_t, SIZE>& attackTable,
                    Function getAttackMask, bool isBishop) {

    int attacksFilled = 0;
    attackTable.fill(ALL_SQUARES); // no true attack will be all squares
    for (int i = 0; i < BOARD_SIZE; i++) {
        // fill magic table index
        const uint64_t blockerMask = getRelevantBlockerMask(i, isBishop);
        table[i].slideMask = blockerMask;
        table[i].magic = magicTable[i];
        table[i].shift = 64 - popcount(blockerMask);
        table[i].offset = attacksFilled;

        // fill attack table entries for the magic index
        std::vector<uint64_t> blockers = getPossibleBlockers(blockerMask);
        for (const uint64_t blocker: blockers) {
            const uint64_t attacks = getAttackMask(i, blocker);
            const int index = getMagicIndex(table[i], blocker);
            if (attackTable[index] != attacks) {
                assert(attackTable[index] == ALL_SQUARES); // checks for illegal collisions
            }
            attackTable[index] = attacks;
        }
        attacksFilled += 1 << popcount(blockerMask);
    } 
}

// Works for bishops and rooks
uint64_t Attacks::getRelevantBlockerMask(int square, bool isBishop) {
    uint64_t slideMask = isBishop ? getDiagMask(square) | getAntiDiagMask(square) :
                                    getFileMask(square) | getRankMask(square);
    // pieces on the edges are blocked by same edge pieces
    slideMask &= square / 8 != 0 ? ~RANK_8 : ALL_SQUARES;
    slideMask &= square / 8 != 7 ? ~RANK_1 : ALL_SQUARES;
    slideMask &= square % 8 != 0 ? ~FILE_A : ALL_SQUARES;
    slideMask &= square % 8 != 7 ? ~FILE_H : ALL_SQUARES;
    // the current square isn't a valid blocker
    slideMask ^= c_u64(1) << square;
    return slideMask;
}

std::vector<uint64_t> Attacks::getPossibleBlockers(uint64_t slideMask) {
    std::vector<uint64_t> blockerBoards;
    std::vector<int> blockerSquares;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((c_u64(1) << i) & slideMask) {
            blockerSquares.push_back(i);
        }
    }

    // binary iteration will be used to iterate through the possible blockers
    const uint64_t numBlockerBoards = c_u64(1) << blockerSquares.size();
    for (uint64_t i = 0; i < numBlockerBoards; i++) {
        uint64_t currBlocker = 0;
        // convert binary iter to blocker bitboard
        for (uint64_t j = 0; j < blockerSquares.size(); j++) {
            const uint64_t currBit = i & (c_u64(1) << j) ? 1 : 0; 
            currBlocker |= currBit << blockerSquares[j];
        }
        blockerBoards.push_back(currBlocker);
    }

    return blockerBoards;
}

uint64_t Attacks::computeRookAttacks(int square, uint64_t blockers) {
    uint64_t attacks = 0;
    attacks |= fillInDir(square, blockers, 0, 1);
    attacks |= fillInDir(square, blockers, 0, -1);
    attacks |= fillInDir(square, blockers, 1, 0);
    attacks |= fillInDir(square, blockers, -1, 0);
    return attacks;
}

uint64_t Attacks::computeBishopAttacks(int square, uint64_t blockers) {
    uint64_t attacks = 0;
    attacks |= fillInDir(square, blockers, 1, 1);
    attacks |= fillInDir(square, blockers, 1, -1);
    attacks |= fillInDir(square, blockers, -1, 1);
    attacks |= fillInDir(square, blockers, -1, -1);
    return attacks;
}

uint64_t Attacks::fillInDir(int square, uint64_t blockers, int x, int y) {
    int currX = square % 8 + x;
    int currY = square / 8 + y;
    uint64_t filled = 0;
    while ( !(filled & blockers) && 
        currX >= 0 && currX < 8 &&
        currY >= 0 && currY < 8) {

        square = 8 * currY + currX;
        filled |= c_u64(1) << square;
        currX += x; 
        currY += y;
    }
    return filled;
}

uint64_t Attacks::computeKnightAttacks(int square) {
    const uint64_t currPiece = c_u64(1) << square;
    const uint64_t left1 = (currPiece >> 1) & NOT_FILE_H;
    const uint64_t left2 = (currPiece >> 2) & NOT_FILE_HG;
    const uint64_t right1 = (currPiece << 1) & NOT_FILE_A;
    const uint64_t right2 = (currPiece << 2) & NOT_FILE_AB;

    const uint64_t height1 = left1 | right1;
    const uint64_t height2 = left2 | right2;
    
    const uint64_t knightSquares = (height1 << 16) | (height1 >> 16) | (height2 << 8) | (height2 >> 8);
    return knightSquares;
}

uint64_t Attacks::computeKingAttacks(int square) {
    uint64_t currPiece = c_u64(1) << square;

    // prevent currPiece from teleporting to other side of the board with bit shifts
    const uint64_t left = currPiece & NOT_FILE_A;
    const uint64_t right = currPiece & NOT_FILE_H;
    
    currPiece |= (left >> 1);
    currPiece |= (right << 1);
    
    // up one row and down one row respectively
    currPiece |= (currPiece >> 8) | (currPiece << 8);
    return currPiece;
}

uint64_t Attacks::computePawnAttacks(int square, bool isWhiteTurn) {
    const uint64_t currPiece = c_u64(1) << square;

    // prevent currPiece from teleporting to other side of the board with bit shifts
    const uint64_t left  = currPiece & NOT_FILE_A;
    const uint64_t right = currPiece & NOT_FILE_H;
    
    const uint64_t upPawns   = (left >> 9) | (right >> 7); 
    const uint64_t downPawns = (left << 7) | (right << 9); 
    return isWhiteTurn ? upPawns : downPawns;
}

// global attack tables
std::array<Attacks::Magic, BOARD_SIZE> Attacks::ROOK_TABLE;
std::array<Attacks::Magic, BOARD_SIZE> Attacks::BISHOP_TABLE;
std::array<uint64_t, 102400> Attacks::ROOK_ATTACKS;
std::array<uint64_t, 5248> Attacks::BISHOP_ATTACKS;
std::array<std::array<uint64_t, BOARD_SIZE>, 2> Attacks::PAWN_ATTACKS;
std::array<uint64_t, BOARD_SIZE> Attacks::KNIGHT_ATTACKS;
std::array<uint64_t, BOARD_SIZE> Attacks::KING_ATTACKS;

// magics generation
void Attacks::generateMagics() {
    RNGSeed seed = {0xf38f4541449b0fc3, 0x8432cf48703f8864, 0x1c8596ae5c1621d1, 0xf6d3be81a796f876};

    int attacksSize = 0;
    std::cout << "Rook attacks: " << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        const uint64_t blockerMask = getRelevantBlockerMask(i, false);
        const int blockersSize = popcount(blockerMask);
        const uint64_t magic = findMagic(computeRookAttacks, i, blockerMask, blockersSize, seed);
        attacksSize += c_u64(1) << blockersSize;
        printHex(magic);
    }
    std::cout << "Rook attacks size: " << attacksSize << std::endl;
    std::cout << std::endl;

    attacksSize = 0;
    std::cout << "Bishop attacks: " << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        const uint64_t blockerMask = getRelevantBlockerMask(i, true);
        const int blockersSize = popcount(blockerMask);
        const uint64_t magic = findMagic(computeBishopAttacks, i, blockerMask, blockersSize, seed);
        attacksSize += c_u64(1) << blockersSize;
        printHex(magic);
    }
    std::cout << "Bishop attacks size: " << attacksSize << std::endl;
}

template<typename Function>
uint64_t Attacks::findMagic(Function slidingAttacks, int square, uint64_t blockerMask, int shift, RNGSeed& seed) {
    std::vector<uint64_t> possibleBlockers = getPossibleBlockers(blockerMask);
    std::array<uint64_t, 4096> moves{};
    uint64_t magic = 0;
    bool magicFound = false;

    while (!magicFound) {
        moves.fill(ALL_SQUARES);
        magicFound = true;
        const size_t maxIndex = c_u64(1) << shift;
        // magic numbers with low number of 1s are better
        magic = rand64(seed) & rand64(seed) & rand64(seed);

        // checks for collisions with all possible blockers for a magic number
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