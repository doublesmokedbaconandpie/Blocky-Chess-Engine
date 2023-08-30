#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>

#include "bitboard.hpp"

int leadingBit(uint64_t bitboard) {
    /*Counts from the first bit*/ 
    return bitboard ? __builtin_ctzll(bitboard) : -1;
}

int trailingBit(uint64_t bitboard) {
    /*Counts from the last bit*/ 
    return bitboard ? __builtin_clzll(bitboard) : -1;
}

uint64_t flipVertical(uint64_t bitboard) {
    uint64_t k1 = 0x00FF00FF00FF00FFull;
    uint64_t k2 = 0x0000FFFF0000FFFFull;
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
    return FILES_MASK.at(getFile(square));
}

uint64_t getRankMask(int square) {
    return RANKS_MASK.at(getRank(square));
}

uint64_t getDiagMask(int square) {
    return DIAGS_MASK.at(getRank(square) + getFile(square));
}

uint64_t getAntiDiagMask(int square) {
    return flipVertical(DIAGS_MASK.at(7 - getRank(square) + getFile(square)));
}

bool diagAttackers(int square, uint64_t allies, uint64_t enemies) {
    allies ^= (1ull << square);

    // 'a' or 'A' signifies anti-diagonal
    uint64_t currDiag  = getDiagMask(square);
    uint64_t aCurrDiag = getAntiDiagMask(square);

    uint64_t allyDiag   = allies  & currDiag;
    uint64_t allyADiag  = allies  & aCurrDiag;
    uint64_t enemyDiag  = enemies & currDiag;
    uint64_t enemyADiag = enemies & aCurrDiag;

    // get rid of pieces in the same diagonal but not part of the section we want
    int nSquare = 64 - square;
    uint64_t urAlly = (allyDiag  << nSquare) >> nSquare;
    uint64_t ulAlly = (allyADiag << nSquare) >> nSquare;
    uint64_t dlAlly = (allyDiag  >> square)  << square;
    uint64_t drAlly = (allyADiag >> square)  << square;

    uint64_t urEnemy = (enemyDiag  << nSquare) >> nSquare;
    uint64_t ulEnemy = (enemyADiag << nSquare) >> nSquare;
    uint64_t dlEnemy = (enemyDiag  >> square)  << square;
    uint64_t drEnemy = (enemyADiag >> square)  << square;

    // leadingBit and trailingBit return -1 on bitboards with no pieces
    uint64_t closestDiags = 0ull;
    closestDiags |= (leadingBit(urAlly) < leadingBit(urEnemy)) ? urEnemy : 0ull;
    closestDiags |= (leadingBit(ulAlly) < leadingBit(ulEnemy)) ? ulEnemy : 0ull;
    closestDiags |= (trailingBit(dlAlly) < trailingBit(dlEnemy)) ? dlEnemy : 0ull;
    closestDiags |= (trailingBit(drAlly) < trailingBit(drEnemy)) ? drEnemy : 0ull;
    return closestDiags & enemies;
}

bool straightAttackers(int square, uint64_t allies, uint64_t enemies) {
    allies ^= (1ull << square);

    uint64_t currFile = getFileMask(square);
    uint64_t currRank = getRankMask(square);

    uint64_t allyFile  = allies  & currFile;
    uint64_t allyRank  = allies  & currRank;
    uint64_t enemyFile = enemies & currFile;
    uint64_t enemyRank = enemies & currRank;

    // get rid of pieces in the same straight but not part of the section we want
    int nSquare = 64 - square;
    uint64_t uAlly = (allyFile << nSquare) >> nSquare;
    uint64_t lAlly = (allyRank << nSquare) >> nSquare;
    uint64_t dAlly = (allyFile >> square)  << square;
    uint64_t rAlly = (allyRank >> square)  << square;

    uint64_t uEnemy = (enemyFile << nSquare) >> nSquare;
    uint64_t lEnemy = (enemyRank << nSquare) >> nSquare;
    uint64_t dEnemy = (enemyFile >> square)  << square;
    uint64_t rEnemy = (enemyRank >> square)  << square;

    // leadingBit and trailingBit return -1 on bitboards with no pieces
    uint64_t closestStraights = 0ull;
    closestStraights |= (leadingBit(uAlly) < leadingBit(uEnemy)) ? uEnemy : 0ull;
    closestStraights |= (leadingBit(lAlly) < leadingBit(lEnemy)) ? lEnemy : 0ull;
    closestStraights |= (trailingBit(dAlly) < trailingBit(dEnemy)) ? dEnemy : 0ull;
    closestStraights |= (trailingBit(rAlly) < trailingBit(rEnemy)) ? rEnemy : 0ull;
    return closestStraights & enemies;
}

bool knightAttackers(int square, uint64_t enemyKnights) {
    uint64_t currPiece = 1ull << square;

    uint64_t left1 = (currPiece >> 1) & NOT_FILE_A;
    uint64_t left2 = (currPiece >> 2) & NOT_FILE_AB;
    uint64_t right1 = (currPiece << 1) & NOT_FILE_H;
    uint64_t right2 = (currPiece << 2) & NOT_FILE_HG;

    uint64_t height1 = left1 | right1;
    uint64_t height2 = left2 | right2;
    
    uint64_t knightSquares = (height1 << 16) | (height1 >> 16) | (height2 << 8) | (height2 >> 8);
    return knightSquares & enemyKnights;
}

bool pawnAttackers(int square, uint64_t enemyPawns, bool isWhiteTurn) {
    uint64_t currPiece = 1ull << square;

    uint64_t left  = currPiece & NOT_FILE_A;
    uint64_t right = currPiece & NOT_FILE_H;
    
    uint64_t upPawns   = (left >> 9) | (right >> 7); 
    uint64_t downPawns = (left << 7) | (right << 9); 
    return isWhiteTurn ? upPawns & enemyPawns : downPawns & enemyPawns;
}

bool kingAttackers(int square, uint64_t enemyKings) {
    uint64_t currPiece = 1ull << square;
    uint64_t left = currPiece & NOT_FILE_A;
    uint64_t right = currPiece & NOT_FILE_H;
    
    currPiece |= (left >> 1);
    currPiece |= (right << 1);
    
    currPiece |= (currPiece >> 8) | (currPiece << 8);
    return currPiece & enemyKings;
}

void printBitboard(uint64_t bitboard) {
    std::cout << "0x" << std::setw(16) << std::setfill('0') << std::hex <<  bitboard << "\n" << std::dec;
}