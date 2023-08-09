
#include "board.hpp"

bool kingInCheck(Board& currBoard) {
    // checks if the current player's king is in check

    std::vector<BoardSquare> pawns, knights, bishops, rooks, queens;

    int offset = 0;
    if (currBoard.isWhiteTurn) {
        offset = 9;
    }
    int targetKing = BKing - offset;

    for (int rank = 0; rank < 8; rank++) {
        for (const auto file : { fileVals::A, fileVals::H } ) {
            BoardSquare currSquare = BoardSquare(file, rank);
            int currPiece = currBoard.getPiece(file, rank);
            if (currPiece == WPawn + offset || WPawnJumped + offset ) {
                pawns.push_back(currSquare);
            }
            if (currPiece == WKnight + offset) {
                knights.push_back(currSquare);
            }
            if (currPiece == WBishop + offset) {
                bishops.push_back(currSquare);
            }
            if (currPiece == WRook + offset) {
                rooks.push_back(currSquare);
            }
            if (currPiece == WQueen + offset) {
                queens.push_back(currSquare);
            }
        }
    }

    // check for pawns
    for (BoardSquare pawn: pawns) {
        int rank = pawn.rank;
        fileVals file = pawn.file;
        int direction = 1;
        if (currBoard.isWhiteTurn) {
            direction = -1;
        }

        if (file == A) {
            if (currBoard.getPiece(file + direction, rank + 1) == BKing - offset) {
                return true;
            }
        }
        else if (file == H) {
            if (currBoard.getPiece(file + direction, rank - 1) == BKing - offset) {
                return true;
            }
        }
        else {
            if (currBoard.getPiece(file + direction, rank - 1) == BKing - offset) {
                return true;
            }
            if (currBoard.getPiece(file + direction, rank + 1) == BKing - offset) {
                return true;
            }
        }
    }  

    // check for bishops
    for (BoardSquare bishop: bishops) {
        // check bottom left
        int currRank = bishop.rank;
        int currFile = bishop.file;
        while (currRank < 8 && currFile >= A) {
            if (currBoard.getPiece(currRank, currFile) != 0) {
                break;
            }
            else if (currBoard.getPiece(currRank, currFile) == BKing - offset) {
                return true;
            }
            currRank++;
            currFile--;
        }

        // check bottom right
        int currRank = bishop.rank;
        int currFile = bishop.file;
        while (currRank < 8 && currFile <= H) {
            if (currBoard.getPiece(currRank, currFile) != 0) {
                break;
            }
            else if (currBoard.getPiece(currRank, currFile) == BKing - offset) {
                return true;
            }
            currRank++;
            currFile++;
        }

        // check top left
        int currRank = bishop.rank;
        int currFile = bishop.file;
        while (currRank > 0 && currFile >= A) {
            if (currBoard.getPiece(currRank, currFile) != 0) {
                break;
            }
            else if (currBoard.getPiece(currRank, currFile) == BKing - offset) {
                return true;
            }
            currRank--;
            currFile--;
        }

        // check top right
        int currRank = bishop.rank;
        int currFile = bishop.file;
        while (currRank > 0 && currFile <= H) {
            if (currBoard.getPiece(currRank, currFile) != 0) {
                break;
            }
            else if (currBoard.getPiece(currRank, currFile) == BKing - offset) {
                return true;
            }
            currRank--;
            currFile++;
        }
    }



    return false;
}