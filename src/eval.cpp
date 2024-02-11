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
#include "move.hpp"
#include "bitboard.hpp"
#include "attacks.hpp"
#include "zobrist.hpp"
#include "utils/types.hpp"

namespace Eval {

int Info::getRawEval(const PieceSets& pieceSets, bool isWhiteTurn) {
    // positive values means white is winning, negative means black
    const S pawnScore = this->getPawnInfo(pieceSets).score;
    const S mobilityScore = evalMobilityScores(pieceSets, true) - evalMobilityScores(pieceSets, false);
    const S tempoBonus = isWhiteTurn ? tempo : -tempo;
    const S totalScore = this->score + pawnScore + mobilityScore + tempoBonus;

    const int op = totalScore.opScore;
    const int eg = totalScore.egScore;
    const int eval = (op * this->phase + eg * (TOTAL_PHASE - this->phase)) / TOTAL_PHASE;

    return eval + evalMopUpScore(pieceSets, eval);
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

S evalPawns(const PieceSets& pieceSets, bool isWhite) {
    const auto allyPawn  = isWhite ? WPawn : BPawn;
    const auto enemyPawn = isWhite ? BPawn : WPawn;

    const auto allyPawnSet = pieceSets[allyPawn];
    const auto enemyPawnSet = pieceSets[enemyPawn];

    int pawn;
    S pawnScore{};

    auto pawns = pieceSets[allyPawn];
    while (pawns) {
        pawn = popLsb(pawns);
        if (isPassedPawn(pawn, enemyPawnSet, isWhite)) {
            const int index = isWhite ? getRank(pawn) : getRank(pawn) ^ 7;
            pawnScore += passedPawn[index];
        }
    }

    const auto doubledPawnsMask = getDoubledPawnsMask(allyPawnSet, isWhite);
    pawnScore += doubledPawns * popcount(doubledPawnsMask);

    const auto chainedPawnsMask = getChainedPawnsMask(allyPawnSet, isWhite);
    pawnScore += chainedPawns * popcount(chainedPawnsMask);

    const auto phalanxPawnsMask = getPhalanxPawnsMask(allyPawnSet);
    pawnScore += phalanxPawns * popcount(phalanxPawnsMask);

    return pawnScore;
}

S evalMobilityScores(const PieceSets& pieceSets, bool isWhite) {
    const uint64_t mobilitySquares = getMobilitySquares(pieceSets, isWhite);
    const uint64_t allPieces   = isWhite ? pieceSets[WHITE_PIECES] : pieceSets[BLACK_PIECES];
    uint64_t allyKnights = isWhite ? pieceSets[WKnight] : pieceSets[BKnight];
    uint64_t allyBishops = isWhite ? pieceSets[WBishop] : pieceSets[BBishop];
    uint64_t allyRooks = isWhite ? pieceSets[WRook] : pieceSets[BRook];

    S mobilityScores{};
    Square sq;

    while (allyKnights) {
        sq = popLsb(allyKnights);
        mobilityScores += knightMobility[getPieceMobility(KNIGHT, sq, mobilitySquares, allPieces)];
    }
    while (allyBishops) {
        sq = popLsb(allyBishops);
        mobilityScores += bishopMobility[getPieceMobility(BISHOP, sq, mobilitySquares, allPieces)];
    }
    while (allyRooks) {
        sq = popLsb(allyRooks);
        mobilityScores += rookMobility[getPieceMobility(ROOK, sq, mobilitySquares, allPieces)];
    }

    return mobilityScores;
}

int evalMopUpScore(const PieceSets& pieceSets, int eval) {
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

bool isPassedPawn(Square pawn, uint64_t enemyPawns, bool isWhite) {
    const int file = getFile(pawn);
    const int rank = getRank(pawn);
    uint64_t adjacentEnemies = ADJ_FILES_AND_FILES_MASK[file] & enemyPawns;
    if (!adjacentEnemies) {
        return true;
    }

    const int backEnemy = isWhite ? popLsb(adjacentEnemies) : popMsb(adjacentEnemies);
    const int enemyRank = getRank(backEnemy);
    return isWhite ? rank <= enemyRank : rank >= enemyRank;
}

// gets all pawns that are in front of other ally pawns
uint64_t getDoubledPawnsMask(uint64_t allyPawnSet, bool isWhite) {
    const uint64_t forwardSquare = isWhite ? allyPawnSet >> 8 : allyPawnSet << 8;
    return allyPawnSet & forwardSquare;
}

// gets all pawns that are defending other ally pawns
uint64_t getChainedPawnsMask(uint64_t allyPawnSet, bool isWhite) {
    // prevent pawns from teleporting to other side of the board with bit shifts
    const uint64_t left  = allyPawnSet & NOT_FILE_A;
    const uint64_t right = allyPawnSet & NOT_FILE_H;

    // find ally pawns in left and right diagonal direction
    const uint64_t leftDiag = isWhite ? left >> 9 : left << 7;
    const uint64_t rightDiag = isWhite ? right >> 7 : right << 9;
    return allyPawnSet & (leftDiag | rightDiag);
}

// get all pawns that have ally pawns to the immediate left
uint64_t getPhalanxPawnsMask(uint64_t allyPawnSet) {
    const uint64_t leftSquare = (allyPawnSet & NOT_FILE_A) >> 1;
    return leftSquare & allyPawnSet;
}

// returns all squares that can be moved to (including captures) that aren't defended by enemy pawns
uint64_t getMobilitySquares(const PieceSets& pieceSets, bool isWhite) {
    const uint64_t nonAllies = isWhite ? ~pieceSets[WHITE_PIECES] : ~pieceSets[BLACK_PIECES];
    const uint64_t enemyPawns = isWhite ? pieceSets[BPawn] : pieceSets[WPawn];
    const uint64_t left  = enemyPawns & NOT_FILE_A;
    const uint64_t right = enemyPawns & NOT_FILE_H;
    const uint64_t attackedSquares = isWhite ?
        (left << 7) | (right << 9):
        (left >> 9) | (right >> 7);
    return nonAllies & ~attackedSquares;
}

int getPieceMobility(pieceTypes piece, Square sq, uint64_t mobilitySquares, uint64_t allPieces) {
    uint64_t movementSquares;
    switch (piece)
    {
    case KNIGHT:
        movementSquares = Attacks::knightAttacks(sq);
        break;
    case BISHOP:
        movementSquares = Attacks::bishopAttacks(sq, allPieces);
        break;
    case ROOK:
        movementSquares = Attacks::rookAttacks(sq, allPieces);
        break;
    default:
        assert(false);
    }
    return popcount(movementSquares & mobilitySquares);
}

} // namespace Eval
