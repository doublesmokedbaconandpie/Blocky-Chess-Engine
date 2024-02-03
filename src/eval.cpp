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
#include <iostream>

#include "eval.hpp"
#include "bitboard.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "zobrist.hpp"
#include "types.hpp"

namespace Eval {

int Info::getRawEval(const PieceSets& pieceSets) {
    // positive values means white is winning, negative means black
    const auto& pawnInfo = this->getPawnInfo(pieceSets);
    const S totalScore = this->score + pawnInfo.score;
    const int op = totalScore.opScore;
    const int eg = totalScore.egScore;
    const int eval = (op * this->phase + eg * (TOTAL_PHASE - this->phase)) / TOTAL_PHASE;
    return eval + mopUpScore(pieceSets, eval);
}

void Info::addPiece(Square square, pieceTypes piece) {
    this->score += getPSQTVal(square, piece);
    this->phase += getPiecePhase(piece);
    if (piece == WPawn || piece == BPawn) {
        this->pawnKey ^= Zobrist::pieceKeys[piece][square];
    }
}

void Info::removePiece(Square square, pieceTypes piece) {
    this->score -= getPSQTVal(square, piece);
    this->phase -= getPiecePhase(piece);
    if (piece == WPawn || piece == BPawn) {
        this->pawnKey ^= Zobrist::pieceKeys[piece][square];
    }
}

const PawnHashEntry& Info::getPawnInfo(const PieceSets& pieceSets) {
    // probe pawn hash table for precomputed values
    // if not found, then compute the pawn values and replace the entry
    PawnHashEntry& entry = this->pawnHashTable[this->pawnKey % PAWN_HASH_SIZE];
    if (this->pawnKey != entry.key) {
        entry.score = S();
        entry.score += evalPawns(pieceSets, true);
        entry.score -= evalPawns(pieceSets, false);
        entry.key = this->pawnKey;
    }
    return entry;
}

S evalPawns(const PieceSets& pieceSets, bool isWhiteTurn) {
    const auto allyPawn  = isWhiteTurn ? WPawn : BPawn;
    const auto enemyPawn = isWhiteTurn ? BPawn : WPawn;

    const auto allyPawnSet = pieceSets[allyPawn];
    const auto enemyPawnSet = pieceSets[enemyPawn];

    int pawn;
    S pawnScore{};

    auto pawns = pieceSets[allyPawn];
    while (pawns) {
        pawn = popLsb(pawns);
        if (isPassedPawn(pawn, enemyPawnSet, isWhiteTurn)) {
            const int index = isWhiteTurn ? getRank(pawn) : getRank(pawn) ^ 7;
            pawnScore += passedPawn[index];
        }
    }

    const auto doubledPawnMask = getDoubledPawnsMask(allyPawnSet, isWhiteTurn);
    pawnScore += doubledPawns * popcount(doubledPawnMask);

    return pawnScore;
}

int mopUpScore(const PieceSets& pieceSets, int eval) {
    // only use mop up for checkmate positions without pawns
    if (std::abs(eval) < 450 || (pieceSets[WPawn] | pieceSets[BPawn]) ) {
        return 0;
    }
    // winning kings have scores boosted for kings approaching each other
    const int winningMopUp = eval > 0 ? 1 : -1;
    const int kingDistance = std::abs(lsb(pieceSets[WKing]) - lsb(pieceSets[BKing]));
    return winningMopUp * (64 - kingDistance) * 5;

}

int getPiecePhase(pieceTypes piece) {
    switch (piece)
    {
        case WKnight:
        case BKnight:
        case WBishop:
        case BBishop:
            return 1;
        case WRook:
        case BRook:
            return 2;
        case WQueen:
        case BQueen:
            return 4;
        default:
            return 0;
    }
}

// assumes that currPiece is not empty
S getPSQTVal(Square square, pieceTypes currPiece) {
    if (currPiece >= WKing && currPiece <= WPawn) {
        return PSQT[currPiece][square];
    }
    return -PSQT[currPiece - BKing][square ^ 56];
}

bool isPassedPawn(Square pawn, uint64_t enemyPawns, bool isWhitePawn) {
    const int file = getFile(pawn);
    const int rank = getRank(pawn);
    uint64_t adjacentEnemies = ADJ_FILES_AND_FILES_MASK[file] & enemyPawns;
    if (!adjacentEnemies) {
        return true;
    }

    const int backEnemy = isWhitePawn ? popLsb(adjacentEnemies) : popMsb(adjacentEnemies);
    const int enemyRank = getRank(backEnemy);
    return isWhitePawn ? rank <= enemyRank : rank >= enemyRank;
}

} // namespace Eval
