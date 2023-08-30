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
    return bitboard ? __builtin_clzll(bitboard) : 64;
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
    uint64_t currDiag = getDiagMask(square);
    uint64_t aCurrDiag = getAntiDiagMask((square));

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

    uint64_t closetDiags = 0ull;
    if (leadingBit(enemyDiag) != -1 && (leadingBit(enemyDiag) < square)) { // up right
        closetDiags |= (leadingBit(urAlly) < leadingBit(enemyDiag)) ? enemyDiag : 0ull;
    }
    if (leadingBit(enemyADiag) != -1 && (leadingBit(enemyADiag) < square)) { // up left
        closetDiags |= (leadingBit(ulAlly) < leadingBit(enemyADiag)) ? enemyADiag : 0ull;
    }
    if (trailingBit(enemyDiag) != 64 && (trailingBit(enemyDiag) < square)) { // down left
        closetDiags |= (trailingBit(dlAlly) > trailingBit(enemyDiag)) ? enemyDiag : 0ull;
    }
    if (trailingBit(enemyADiag) != 64 && (trailingBit(enemyADiag) < square)) { // down right
        closetDiags |= (trailingBit(drAlly) > trailingBit(enemyADiag)) ? enemyADiag : 0ull;
    }
    return closetDiags & enemies;
}

bool straightAttackers(int square, uint64_t allies, uint64_t enemies) {
    allies ^= (1ull << square);

    uint64_t currFile = getFileMask(square);
    uint64_t currRank = getRankMask(square);

    uint64_t allyFile  = allies  & currFile;
    uint64_t allyRank  = allies  & currRank;
    uint64_t enemyFile = enemies & currFile;
    uint64_t enemyRank = enemies & currRank;

    // get rid of pieces in the same diagonal but not part of the section we want
    int nSquare = 64 - square;
    uint64_t uAlly = (allyFile << nSquare) >> nSquare;
    uint64_t lAlly = (allyRank << nSquare) >> nSquare;
    uint64_t dAlly = (allyFile >> square)  << square;
    uint64_t rAlly = (allyRank >> square)  << square;

    uint64_t closestStraights = 0ull;
    if (leadingBit(enemyFile) != -1 && (leadingBit(enemyFile) < square)) { // up
        closestStraights |= (leadingBit(uAlly) < leadingBit(enemyFile)) ? enemyFile : 0ull;
    }
    if (leadingBit(enemyRank) != -1 && (leadingBit(enemyFile) < square)) { // left
        closestStraights |= (leadingBit(lAlly) < leadingBit(enemyRank)) ? enemyRank : 0ull;
    }
    if (trailingBit(enemyFile) != 64 && (trailingBit(enemyFile) < square)) { // down
        closestStraights |= (trailingBit(dAlly) > trailingBit(enemyFile)) ? enemyFile : 0ull;
    }
    if (trailingBit(enemyRank) != 64 && (trailingBit(enemyFile) < square)) { // right
        closestStraights |= (trailingBit(rAlly) > trailingBit(enemyRank)) ? enemyRank : 0ull;  
    }
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