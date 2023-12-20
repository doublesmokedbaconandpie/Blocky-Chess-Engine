#include <array>
#include <iostream>

#include "eval.hpp"
#include "bitboard.hpp"
#include "move.hpp"
#include "types.hpp"

namespace Eval {

int Info::getRawEval(const PieceSets& pieceSets) const {
    // positive values means white is winning, negative means black
    int score = (this->opScore * phase + this->egScore * (totalPhase - phase)) / totalPhase;
    return score + mopUpScore(pieceSets, score);
}

void Info::addPiece(Square square, pieceTypes piece) {
    this->opScore += Eval::getPlacementScoreOp(square, piece);
    this->egScore += Eval::getPlacementScoreEg(square, piece);
    this->phase += getPiecePhase(piece);
}

void Info::removePiece(Square square, pieceTypes piece) {
    this->opScore -= Eval::getPlacementScoreOp(square, piece);
    this->egScore -= Eval::getPlacementScoreEg(square, piece);
    this->phase -= getPiecePhase(piece);
}

int Info::mopUpScore(const PieceSets& pieceSets, int score) const {
    // only use mop up for checkmate positions without pawns
    if (std::abs(score) < 450 || (pieceSets[WPawn] | pieceSets[BPawn]) ) {
        return 0;
    }
    // winning kings have scores boosted for kings approaching each other
    int winningMopUp = score > 0 ? 1 : -1;
    int kingDistance = std::abs(lsb(pieceSets[WKing]) - lsb(pieceSets[BKing]));
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
int getPlacementScoreOp(Square square, pieceTypes currPiece) {
    if(currPiece >= WKing && currPiece <= WPawn) {
        return tablesOp[currPiece][square];
    }
    return -tablesOp[currPiece - BKing][square ^ 56];
}

// assumes that currPiece is not empty
int getPlacementScoreEg(Square square, pieceTypes currPiece) {
    if(currPiece >= WKing && currPiece <= WPawn) {
        return tablesEg[currPiece][square];
    }
    return -tablesEg[currPiece - BKing][square ^ 56];
}

} // namespace Eval