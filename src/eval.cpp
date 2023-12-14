#include <array>
#include <cassert>
#include <iostream>

#include "eval.hpp"
#include "bitboard.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "types.hpp"

namespace Eval {

int Info::getRawEval(const PieceSets& pieceSets) const {
    // positive values means white is winning, negative means black
    int op = this->opScore + evalPawns<true>(pieceSets);
    int eg = this->egScore + evalPawns<false>(pieceSets);
    int score = (op * phase + eg * (totalPhase - phase)) / totalPhase;
    return score + mopUpScore(pieceSets, score);
}

template<bool ISOPENING>
int Info::evalPawns(const PieceSets& pieceSets) const {
    int score = 0;

    score += evalPassedPawns<ISOPENING, true>(pieceSets);
    score -= evalPassedPawns<ISOPENING, false>(pieceSets);

    return score;
}

template<bool ISOPENING, bool ISWHITE>
int Info::evalPassedPawns(const PieceSets& pieceSets) const {
    constexpr auto PASSED_PAWNS = ISOPENING ? passedPawnOp : passedPawnEg;
    constexpr auto allyPawn  = ISWHITE ? WPawn : BPawn;
    constexpr auto enemyPawn = ISWHITE ? BPawn : WPawn;

    auto allyPawnSet = pieceSets[allyPawn];
    const auto enemyPawnSet = pieceSets[enemyPawn];

    int score = 0, pawn;

    while (allyPawnSet) {
        pawn = popLsb(allyPawnSet);
        if (isPassedPawn(pawn, enemyPawnSet, ISWHITE)) {
            score += PASSED_PAWNS[getFile(pawn)];
        }
    }

    return score;
}

void Info::addPiece(Square square, pieceTypes piece) {
    this->opScore += Eval::getPlacementScore<true>(square, piece);
    this->egScore += Eval::getPlacementScore<false>(square, piece);
    this->phase += getPiecePhase(piece);
}

void Info::removePiece(Square square, pieceTypes piece) {
    this->opScore -= Eval::getPlacementScore<true>(square, piece);
    this->egScore -= Eval::getPlacementScore<false>(square, piece);
    this->phase -= getPiecePhase(piece);
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