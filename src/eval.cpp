
#include "eval.hpp"
#include "board.hpp"

int eval(Board board) {
    int scoreSum = 0;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = A; file <= H; file++) {
            int piece = board.getPiece(rank, file);
            BoardSquare currSquare = BoardSquare(rank, file);
            switch (piece)
            {
            case WPawn:
            case WPawnJumped:
                scoreSum += 1;
                break;
            case WKnight:
            case WBishop:
                scoreSum += 3;
                break;
            case WRook:
            case WRookUnmoved:
                scoreSum += 5;
                break;
            case WQueen:
                scoreSum += 9;
                break;
            case BPawn:
            case BPawnJumped:
                scoreSum -= 1;
                break;
            case BKnight:
            case BBishop:
                scoreSum -= 3;
                break;
            case BRook:
            case BRookUnmoved:
                scoreSum -= 5;
                break;
            case BQueen:
                scoreSum -= 9;
                break;
            default:
                break;
            }
        }
    }
    return board.isWhiteTurn ? scoreSum : scoreSum * -1;
}