#include <array>

#include "eval.hpp"
#include "board.hpp"



int eval(Board board) {
    int scoreSum = 0;

    scoreSum += board.materialDifference * 100;
    scoreSum += board.eval.placementScore;
    
    return board.isWhiteTurn ? scoreSum : scoreSum * -1;
}

int getPlacementScore(int rank, int file, pieceTypes currPiece) {
    if(currPiece >= WKing && currPiece <= WPawn) {
        return tablesOp[currPiece][rank * 8 + file];
    }
    else if(currPiece >= BKing && currPiece <= BPawn) {
        return -1 * tablesOp[currPiece - BKing][file + 56 - 8 * rank];
    }
    
    return 0;
}