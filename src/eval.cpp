#include <array>
#include <iostream>

#include "eval.hpp"
#include "types.hpp"

namespace Eval {

// global variables
std::array<std::array<int, BOARD_SIZE>, 6> tablesOp = {tableKingOp, tableQueenOp, tableBishopOp, tableKnightOp, tableRookOp, tablePawnOp};
std::array<std::array<int, BOARD_SIZE>, 6> tablesEg = {tableKingEg, tableQueenEg, tableBishopEg, tableKnightEg, tableRookEg, tablePawnEg};

void init() {
    for (int i = WKing; i <= WPawn; i++) {
        int pieceVal = pieceValues[i] * 100;
        for (int j = 0; j < BOARD_SIZE; j++) {
            tablesOp[i][j] += pieceVal;
            tablesEg[i][j] += pieceVal;
        }
    }
}

int Info::getRawEval() const {
    // positive values means white is winning, negative means black
    return (this->opScore * phase + this->egScore * (totalPhase - phase)) / totalPhase;
}

void Info::addPiece(int rank, int file, pieceTypes piece) {
    int pieceColor = isWhitePiece(piece) ? 1 : -1;
    this->opScore += Eval::getPlacementScoreOp(rank, file, piece);
    this->egScore += Eval::getPlacementScoreEg(rank, file, piece);
    this->phase += getPiecePhase(piece);
    this->totalMaterial += pieceValues[piece];
    this->materialDifference += pieceValues[piece] * pieceColor;
    this->piecesRemaining++;
}

void Info::removePiece(int rank, int file, pieceTypes piece) {
    int pieceColor = isWhitePiece(piece) ? 1 : -1;
    this->opScore -= Eval::getPlacementScoreOp(rank, file, piece);
    this->egScore -= Eval::getPlacementScoreEg(rank, file, piece);
    this->phase -= getPiecePhase(piece);
    this->totalMaterial -= pieceValues[piece];
    this->materialDifference -= pieceValues[piece] * pieceColor;
    this->piecesRemaining--;
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
int getPlacementScoreOp(int rank, int file, pieceTypes currPiece) {
    if(currPiece >= WKing && currPiece <= WPawn) {
        return tablesOp[currPiece][rank * 8 + file];
    }
    return -1 * tablesOp[currPiece - BKing][file + 56 - 8 * rank];
}

// assumes that currPiece is not empty
int getPlacementScoreEg(int rank, int file, pieceTypes currPiece) {
    if(currPiece >= WKing && currPiece <= WPawn) {
        return tablesEg[currPiece][rank * 8 + file];
    }
    return -1 * tablesEg[currPiece - BKing][file + 56 - 8 * rank];
}

} // namespace Eval