#include <stdexcept>

#include "inCheck.hpp"

bool isWhite(pieceTypes piece) {
    return (piece >= WKing && piece <= WRookUnmoved);
}

bool sameSide(pieceTypes piece1, pieceTypes piece2) {
    pieceTypes whiteBot = WKing;
    pieceTypes whiteTop = WRookUnmoved;
    pieceTypes blackBot = BKing;
    pieceTypes blackTop = BRookUnmoved;

    if (whiteBot <= piece1 && piece1 <= whiteTop) {
        if (whiteBot <= piece2 && piece2 <= whiteTop) {
            return true;
        }
    }
    if (blackBot <= piece1 && piece1 <= blackTop) {
        if (blackBot <= piece2 && piece2 <= blackTop) {
            return true;
        }
    }
    return false;   
}


void addMovesInDirection(Board& currBoard, std::vector<BoardSquare>& movesVec, BoardSquare originSquare, int rankIncrement, int fileIncrement) {
    // static/non-moves will not be appended
    if (rankIncrement == 0 && fileIncrement == 0) {
        throw std::invalid_argument("rankIncrement or fileIncrement must not be 0");
    }
    
    pieceTypes currPiece;
    pieceTypes originPiece = currBoard.getPiece(originSquare);
    int currRank = originSquare.rank + rankIncrement;
    int currFile = originSquare.file + fileIncrement;

    while(currRank >= 0 && currRank <= 7 && currFile >= A && currFile <= H) {
        currPiece = currBoard.getPiece(currRank, currFile);
        if (sameSide(originPiece, currPiece)) {
            break;
        }
        movesVec.push_back(BoardSquare(currRank, currFile));
        if (currPiece != EmptyPiece) {
            break;
        }
        currRank += rankIncrement;
        currFile += fileIncrement;
    }
}

pieceTypes getPieceInDirection(Board& currBoard, BoardSquare originSquare, int rankIncrement, int fileIncrement) {
    if (rankIncrement == 0 && fileIncrement == 0) {
        throw std::invalid_argument("rankIncrement or fileIncrement must not be 0");
    }
    
    pieceTypes currPiece;
    pieceTypes originPiece = currBoard.getPiece(originSquare);
    int currRank = originSquare.rank + rankIncrement;
    int currFile = originSquare.file + fileIncrement;

    while(currRank >= 0 && currRank <= 7 && currFile >= A && currFile <= H) {
        currPiece = currBoard.getPiece(currRank, currFile);
        if (currPiece != EmptyPiece) {
            return currPiece;
        }
        currRank += rankIncrement;
        currFile += fileIncrement;
    }
    return EmptyPiece;
}

bool checkDiagPin(pieceTypes origin, pieceTypes endPiece1, pieceTypes endPiece2) {
    // checks if origin piece is between an ally king and attacking enemy piece
    pieceTypes allyKing, enemyQueen, enemyBishop;
    if (isWhite(origin)) {
        allyKing = WKing;
        enemyQueen = BQueen;
        enemyBishop = BBishop;
    }
    else {
        allyKing = BKing;
        enemyQueen = WQueen;
        enemyBishop = WBishop;
    }

    if (endPiece1 == allyKing || endPiece2 == allyKing) {
        if (endPiece1 == enemyQueen || endPiece1 == enemyBishop ||
            endPiece2 == enemyQueen || endPiece2 == enemyBishop) {
            return true;
        }
    }
    return false;
}
bool checkStraightPin(pieceTypes origin, pieceTypes endPiece1, pieceTypes endPiece2) {
    // checks if origin piece is between an ally king and attacking enemy piece
    pieceTypes allyKing, enemyQueen, enemyRook, enemyRookUnmoved;
    if (isWhite(origin)) {
        allyKing = WKing;
        enemyQueen = BQueen;
        enemyRook = BRook;
        enemyRookUnmoved = BRookUnmoved;
    }
    else {
        allyKing = BKing;
        enemyQueen = WQueen;
        enemyRook = WRook;
        enemyRookUnmoved = WRookUnmoved;
    }

    if (endPiece1 == allyKing || endPiece2 == allyKing) {
        if (endPiece1 == enemyQueen || endPiece1 == enemyRook || endPiece1 == enemyRookUnmoved ||
            endPiece2 == enemyQueen || endPiece2 == enemyRook || endPiece2 == enemyRookUnmoved) {
            return true;
        }
    }
    return false;
}
