#include <algorithm>
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

bool checkDiagAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece) {
    std::vector<pieceTypes> possibleAttackers;
    if (currBoard.isWhiteTurn) {
        possibleAttackers.push_back(BQueen);
        possibleAttackers.push_back(BBishop);
    }
    else {
        possibleAttackers.push_back(WQueen);
        possibleAttackers.push_back(WBishop);
    }

    std::vector<pieceTypes> cornerPieces; 
    cornerPieces.push_back(getPieceInDirection(currBoard, originSquare, 1, -1)); // down left
    cornerPieces.push_back(getPieceInDirection(currBoard, originSquare, 1, 1)); // down right
    cornerPieces.push_back(getPieceInDirection(currBoard, originSquare, -1, -1)); // up left
    cornerPieces.push_back(getPieceInDirection(currBoard, originSquare, -1, 1)); // up right
    
    for (pieceTypes piece: cornerPieces) {
        if (std::find(possibleAttackers.begin(), possibleAttackers.end(), piece) != possibleAttackers.end()) {
            return true;
        }
    }
    return false;
}

bool checkStraightAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece) {
    std::vector<pieceTypes> possibleAttackers;
    if (currBoard.isWhiteTurn) {
        possibleAttackers.push_back(BQueen);
        possibleAttackers.push_back(BRook);
        possibleAttackers.push_back(BRookUnmoved);
    }
    else {
        possibleAttackers.push_back(WQueen);
        possibleAttackers.push_back(WRook);
        possibleAttackers.push_back(WRookUnmoved);
    }

    std::vector<pieceTypes> crossPieces; 
    crossPieces.push_back(getPieceInDirection(currBoard, originSquare, 1, 0)); // down
    crossPieces.push_back(getPieceInDirection(currBoard, originSquare, -1, 0)); // up
    crossPieces.push_back(getPieceInDirection(currBoard, originSquare, 0, -1)); // left
    crossPieces.push_back(getPieceInDirection(currBoard, originSquare, 0, 1)); // right
    
    for (pieceTypes piece: crossPieces) {
        if (std::find(possibleAttackers.begin(), possibleAttackers.end(), piece) != possibleAttackers.end()) {
            return true;
        }
    }
    return false;
}

bool inCheck(Board currBoard) {
    pieceTypes allyKing = currBoard.isWhiteTurn ? WKing : BKing;
    pieceTypes allyKingUnmoved = currBoard.isWhiteTurn ? WKingUnmoved : BKingUnmoved;

    BoardSquare allyKingSquare = BoardSquare();
    for (int rank = 0; rank <= 7; rank++) {
        if (allyKingSquare.file != nullFile) {
            break;
        }
        for (int file = A; file <= H; file++) {
            pieceTypes currPiece = currBoard.getPiece(rank, file);
            if (currPiece == allyKing || currPiece == allyKingUnmoved) {
                allyKingSquare = BoardSquare(rank, file);
                break;
            }
        }
    }

    return checkDiagAttackers(currBoard, allyKingSquare, allyKing) && checkStraightAttackers(currBoard, allyKingSquare, allyKing);

}