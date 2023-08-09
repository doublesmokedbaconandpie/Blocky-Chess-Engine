
#include <vector>
#include <utility>

#include "inCheck.hpp"

bool InCheck(Board& currBoard) {
    // checks if the current player's king is in check

    std::vector<BoardSquare> pawns, knights, bishops, rooks, queens;

    int offset = 0;
    if (currBoard.isWhiteTurn) {
        offset = 9;
    }

    // gets opposing side's pieces
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

    int targetKing = BKing;
    if (currBoard.isWhiteTurn) {
        targetKing = WKing;
    }

    for (BoardSquare pawn: pawns) {
        if (checkPawn(currBoard, pawn, targetKing)) {
            return true;
        }
    }  

    for (BoardSquare bishop: bishops) {
        if (checkBishop(currBoard, bishop, targetKing)) {
            return true;
        }
    }

    for (BoardSquare rook: rooks) {
        if (checkRook(currBoard, rook, targetKing)) {
            return true;
        }
    }

    for (BoardSquare queen: queens) {
        if (!checkBishop(currBoard, queen, targetKing)) {
            continue;
        }
        if (!checkRook(currBoard, queen, targetKing)) {
            continue;
        }
        return true;
    }



    return false;
}

bool checkPawn(Board& currBoard, BoardSquare pawn, int targetKing) {
    int rank = pawn.rank;
    fileVals file = pawn.file;
    int direction = 1;
    if (currBoard.isWhiteTurn) {
        direction = -1;
    }

    if (file == A) {
        if (currBoard.getPiece(file + direction, rank + 1) == targetKing) {
            return true;
        }
    }
    else if (file == H) {
        if (currBoard.getPiece(file + direction, rank - 1) == targetKing) {
            return true;
        }
    }
    else {
        if (currBoard.getPiece(file + direction, rank - 1) == targetKing) {
            return true;
        }
        if (currBoard.getPiece(file + direction, rank + 1) == targetKing) {
            return true;
        }
    }
}

bool checkBishop(Board& currBoard, BoardSquare bishop, int targetKing) {
    // check bottom left
    int currRank = bishop.rank;
    int currFile = bishop.file;
    while (currRank <= 7 && currFile >= A) {
        if (currBoard.getPiece(currRank, currFile) != 0) {
            break;
        }
        else if (currBoard.getPiece(currRank, currFile) == targetKing) {
            return true;
        }
        currRank++;
        currFile--;
    }

    // check bottom right
    currRank = bishop.rank;
    currFile = bishop.file;
    while (currRank <= 7 && currFile <= H) {
        if (currBoard.getPiece(currRank, currFile) != 0) {
            break;
        }
        else if (currBoard.getPiece(currRank, currFile) == targetKing) {
            return true;
        }
        currRank++;
        currFile++;
    }

    // check top left
    currRank = bishop.rank;
    currFile = bishop.file;
    while (currRank >= 0 && currFile >= A) {
        if (currBoard.getPiece(currRank, currFile) != 0) {
            break;
        }
        else if (currBoard.getPiece(currRank, currFile) == targetKing) {
            return true;
        }
        currRank--;
        currFile--;
    }

    // check top right
    currRank = bishop.rank;
    currFile = bishop.file;
    while (currRank >= 0 && currFile <= H) {
        if (currBoard.getPiece(currRank, currFile) != 0) {
            break;
        }
        else if (currBoard.getPiece(currRank, currFile) == targetKing) {
            return true;
        }
        currRank--;
        currFile++;
    }
}

bool checkRook(Board& currBoard, BoardSquare rook, int targetKing) {
    // check down
    int currRank = rook.rank;
    int currFile = rook.file;
    while (currRank <= 7) {
        if (currBoard.getPiece(currRank, currFile) != 0) {
            break;
        }
        else if (currBoard.getPiece(currRank, currFile) == targetKing) {
            return true;
        }
        currRank++;
    }

    // check up
    currRank = rook.rank;
    currFile = rook.file;
    while (currRank >= 0) {
        if (currBoard.getPiece(currRank, currFile) != 0) {
            break;
        }
        else if (currBoard.getPiece(currRank, currFile) == targetKing) {
            return true;
        }
        currRank--;
    }

    // check left
    currRank = rook.rank;
    currFile = rook.file;
    while (currFile >= A) {
        if (currBoard.getPiece(currRank, currFile) != 0) {
            break;
        }
        else if (currBoard.getPiece(currRank, currFile) == targetKing) {
            return true;
        }
        currFile--;
    }

    // check right
    currRank = rook.rank;
    currFile = rook.file;
    while (currFile <= H) {
        if (currBoard.getPiece(currRank, currFile) != 0) {
            break;
        }
        else if (currBoard.getPiece(currRank, currFile) == targetKing) {
            return true;
        }
        currFile++;
    }
}

bool checkKnight(Board& currBoard, BoardSquare knight, int targetKing) {
    int file = knight.file;
    int rank = knight.rank;

    std::vector<std::pair<int, int>> possibleSquares;
    possibleSquares.push_back(std::pair<int, int>(rank - 2, file - 1));
    possibleSquares.push_back(std::pair<int, int>(rank - 2, file + 1));
    possibleSquares.push_back(std::pair<int, int>(rank - 1, file + 2));
    possibleSquares.push_back(std::pair<int, int>(rank + 1, file + 2));
    possibleSquares.push_back(std::pair<int, int>(rank + 2, file + 1));
    possibleSquares.push_back(std::pair<int, int>(rank + 2, file - 1));
    possibleSquares.push_back(std::pair<int, int>(rank + 1, file - 2));
    possibleSquares.push_back(std::pair<int, int>(rank - 1, file - 2));

    for (auto square: possibleSquares) {
        int currRank = square.first;
        int currFile = square.second;
        if (currRank < 0 || currRank > 7) {
            continue;
        }
        if (currFile < A || currFile > H) {
            continue;
        }
        if (currBoard.getPiece(currRank, currFile) == targetKing) {
            
        }
    }
}