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
    const int eval = (op * phase + eg * (totalPhase - phase)) / totalPhase;
    return eval + mopUpScore(pieceSets, eval);
}

void Info::addPiece(Square square, pieceTypes piece) {
    this->score += Eval::getPlacementScore(square, piece);
    this->phase += getPiecePhase(piece);
    if (piece == WPawn || piece == BPawn) {
        this->pawnKey ^= Zobrist::pieceKeys[piece][square];
    }
}

void Info::removePiece(Square square, pieceTypes piece) {
    this->score -= Eval::getPlacementScore(square, piece);
    this->phase -= getPiecePhase(piece);
    if (piece == WPawn || piece == BPawn) {
        this->pawnKey ^= Zobrist::pieceKeys[piece][square];
    }
}

int Info::mopUpScore(const PieceSets& pieceSets, int eval) const {
    // only use mop up for checkmate positions without pawns
    if (std::abs(eval) < 450 || (pieceSets[WPawn] | pieceSets[BPawn]) ) {
        return 0;
    }
    // winning kings have scores boosted for kings approaching each other
    const int winningMopUp = eval > 0 ? 1 : -1;
    const int kingDistance = std::abs(lsb(pieceSets[WKing]) - lsb(pieceSets[BKing]));
    return winningMopUp * (64 - kingDistance) * 5;

}

const PawnHashEntry& Info::getPawnInfo(const PieceSets& pieceSets) {
    // probe pawn hash table for precomputed values
    // if not found, then compute the pawn values and replace the entry
    PawnHashEntry& entry = this->pawnHashTable[this->pawnKey % PAWN_HASH_SIZE];
    if (this->pawnKey != entry.key) {
        entry.score = S();
        entry.score += evalPassedPawns(pieceSets, true);
        entry.score -= evalPassedPawns(pieceSets, false);
        entry.key = this->pawnKey;
    }
    return entry;
}

S Info::evalPassedPawns(const PieceSets& pieceSets, bool isWhiteTurn) const {
    const auto allyPawn  = isWhiteTurn ? WPawn : BPawn;
    const auto enemyPawn = isWhiteTurn ? BPawn : WPawn;

    auto allyPawnSet = pieceSets[allyPawn];
    const auto enemyPawnSet = pieceSets[enemyPawn];

    int pawn;
    S pawnScore{};

    while (allyPawnSet) {
        pawn = popLsb(allyPawnSet);
        if (isPassedPawn(pawn, enemyPawnSet, isWhiteTurn)) {
            const int index = isWhiteTurn ? getRank(pawn) : getRank(pawn) ^ 7;
            pawnScore += passedPawnTable[index];
        }
    }

    return pawnScore;
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
S getPlacementScore(Square square, pieceTypes currPiece) {
    if (currPiece >= WKing && currPiece <= WPawn) {
        return PSQT[currPiece][square];
    }
    return -PSQT[currPiece - BKing][square ^ 56];
}

} // namespace Eval
