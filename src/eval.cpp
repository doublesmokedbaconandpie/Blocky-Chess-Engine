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
    const int op = this->opScore + pawnInfo.opScore;
    const int eg = this->egScore + pawnInfo.egScore;
    const int score = (op * phase + eg * (totalPhase - phase)) / totalPhase;
    return score + mopUpScore(pieceSets, score);
}

void Info::addPiece(Square square, pieceTypes piece) {
    this->opScore += Eval::getPlacementScore<true>(square, piece);
    this->egScore += Eval::getPlacementScore<false>(square, piece);
    this->phase += getPiecePhase(piece);
    if (piece == WPawn || piece == BPawn) {
        this->pawnKey ^= Zobrist::pieceKeys[piece][square];
    }
}

void Info::removePiece(Square square, pieceTypes piece) {
    this->opScore -= Eval::getPlacementScore<true>(square, piece);
    this->egScore -= Eval::getPlacementScore<false>(square, piece);
    this->phase -= getPiecePhase(piece);
    if (piece == WPawn || piece == BPawn) {
        this->pawnKey ^= Zobrist::pieceKeys[piece][square];
    }
}

int Info::mopUpScore(const PieceSets& pieceSets, int score) const {
    // only use mop up for checkmate positions without pawns
    if (std::abs(score) < 450 || (pieceSets[WPawn] | pieceSets[BPawn]) ) {
        return 0;
    }
    // winning kings have scores boosted for kings approaching each other
    const int winningMopUp = score > 0 ? 1 : -1;
    const int kingDistance = std::abs(lsb(pieceSets[WKing]) - lsb(pieceSets[BKing]));
    return winningMopUp * (64 - kingDistance) * 5;

}

const PawnHashEntry& Info::getPawnInfo(const PieceSets& pieceSets) {
    // probe pawn hash table for precomputed values
    // if not found, then compute the pawn values and replace the entry
    PawnHashEntry& entry = this->pawnHashTable[this->pawnKey % PAWN_HASH_SIZE];
    if (this->pawnKey != entry.key) {
        entry.opScore = entry.egScore = 0;
        entry.opScore += evalPassedPawns<true>(pieceSets, true);
        entry.opScore -= evalPassedPawns<true>(pieceSets, false);
        entry.egScore += evalPassedPawns<false>(pieceSets, true);
        entry.egScore -= evalPassedPawns<false>(pieceSets, false);
        entry.key = this->pawnKey;
    }
    return entry;
}

template<bool ISOPENING>
int Info::evalPassedPawns(const PieceSets& pieceSets, bool isWhiteTurn) const {
    constexpr auto& PASSED_PAWNS = ISOPENING ? passedPawnOp : passedPawnEg;
    const auto allyPawn  = isWhiteTurn ? WPawn : BPawn;
    const auto enemyPawn = isWhiteTurn ? BPawn : WPawn;

    auto allyPawnSet = pieceSets[allyPawn];
    const auto enemyPawnSet = pieceSets[enemyPawn];

    int score = 0, pawn;

    while (allyPawnSet) {
        pawn = popLsb(allyPawnSet);
        if (isPassedPawn(pawn, enemyPawnSet, isWhiteTurn)) {
            const int index = isWhiteTurn ? getRank(pawn) : getRank(pawn) ^ 7;
            score += PASSED_PAWNS[index];
        }
    }

    return score;
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
template<bool IS_OPENING>
int getPlacementScore(Square square, pieceTypes currPiece) {
    constexpr auto& tables = IS_OPENING ? tablesOp : tablesEg;
    if (currPiece >= WKing && currPiece <= WPawn) {
        return tables[currPiece][square];
    }
    return -tables[currPiece - BKing][square ^ 56];
}

} // namespace Eval
