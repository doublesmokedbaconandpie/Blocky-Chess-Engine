#include <algorithm>
#include <stdexcept>

#include "inCheck.hpp"

//checks if a square has a friendly piece, useful for determining valid moves
bool isFriendlyPiece(Board& currBoard, BoardSquare targetSquare) {
    int target = currBoard.board.at(targetSquare.rank).at(targetSquare.file); 
    //rank could be wrong depending on how indexing works, could be (7 - rank) instead 

    if(currBoard.isWhiteTurn) {
        return target >= WKing && target <= WRookUnmoved;
    }
    else {
        return target >= BKing && target <= BRookUnmoved;
    }
}

void addMovesInDirection(Board& currBoard, std::vector<BoardSquare>& movesVec, BoardSquare originSquare, int rankIncrement, int fileIncrement) {
    // static/non-moves will not be appended
    if (rankIncrement == 0 && fileIncrement == 0) {
        throw std::invalid_argument("rankIncrement or fileIncrement must not be 0");
    }
    
    pieceTypes currPiece;
    int currRank = originSquare.rank + rankIncrement;
    int currFile = originSquare.file + fileIncrement;

    while(currRank >= 0 && currRank <= 7 && currFile >= A && currFile <= H) {
        BoardSquare currSquare = BoardSquare(currRank, currFile);
        currPiece = currBoard.getPiece(currSquare);
        if (isFriendlyPiece(currBoard, currSquare)) {
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



bool checkKnightAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece) {

}

bool checkPawnAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece) {
    std::vector<pieceTypes> possibleAttackers;
    int direction; // rank direction of enemy pawns that can attack you
    if (currBoard.isWhiteTurn) {
        possibleAttackers.push_back(BPawn);
        possibleAttackers.push_back(BPawnJumped);
        direction = -1;
    }
    else {
        possibleAttackers.push_back(WPawn);
        possibleAttackers.push_back(WPawnJumped);
        direction = 1;
    }

    std::vector<pieceTypes> pawnPieces; 
    int rank = originSquare.rank;
    int file = originSquare.file;
    if (file == A) {
        pawnPieces.push_back(currBoard.getPiece(rank + direction, file + 1));
    }
    else if (file == H) {
        pawnPieces.push_back(currBoard.getPiece(rank + direction, file - 1));
    }
    else {
        pawnPieces.push_back(currBoard.getPiece(rank + direction, file - 1));
        pawnPieces.push_back(currBoard.getPiece(rank + direction, file + 1));
    }

    for (pieceTypes piece: pawnPieces) {
        if (std::find(possibleAttackers.begin(), possibleAttackers.end(), piece) != possibleAttackers.end()) {
            return true;
        }
    }
    return false;
}

bool checkKingAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece) {
    std::vector<pieceTypes> possibleAttackers;
    if (currBoard.isWhiteTurn) {
        possibleAttackers.push_back(BKing);
        possibleAttackers.push_back(BKingUnmoved);
    }
    else {
        possibleAttackers.push_back(WKing);
        possibleAttackers.push_back(WKingUnmoved);
    }

    std::vector<pieceTypes> adjacentPieces; 
    int rank = originSquare.rank;
    int file = originSquare.file;
    adjacentPieces.push_back(currBoard.getPiece(rank + 1, file + 1));
    adjacentPieces.push_back(currBoard.getPiece(rank + 1, file));
    adjacentPieces.push_back(currBoard.getPiece(rank + 1, file - 1));
    adjacentPieces.push_back(currBoard.getPiece(rank, file + 1));
    adjacentPieces.push_back(currBoard.getPiece(rank, file - 1));
    adjacentPieces.push_back(currBoard.getPiece(rank - 1, file - 1));
    adjacentPieces.push_back(currBoard.getPiece(rank - 1, file));
    adjacentPieces.push_back(currBoard.getPiece(rank - 1, file + 1));

    for (pieceTypes piece: adjacentPieces) {
        if (std::find(possibleAttackers.begin(), possibleAttackers.end(), piece) != possibleAttackers.end()) {
            return true;
        }
    }
    return false;
}


bool isInCheck(Board currBoard) {
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
    return checkDiagAttackers(currBoard, allyKingSquare, allyKing) 
        || checkStraightAttackers(currBoard, allyKingSquare, allyKing)
        || checkKnightAttackers(currBoard, allyKingSquare, allyKing)
        || checkPawnAttackers(currBoard, allyKingSquare, allyKing)
        || checkKingAttackers(currBoard, allyKingSquare, allyKing);
}

Board::Board(Board originalBoard, BoardSquare pos1, BoardSquare pos2) {
    this->board = originalBoard.board;
    this->isWhiteTurn = !originalBoard.isWhiteTurn;

    pieceTypes startPiece = this->board.at(pos1.rank).at(pos1.file);
    this->board.at(pos1.rank).at(pos1.file) = EmptyPiece;
    this->board.at(pos2.rank).at(pos2.file) = startPiece;

    this->inCheck = isInCheck(*this);

    // need to add logic to convert pawnJumped, rookUnmoved, and kingUnmoved to their respective pieces
    // perform castling
    // also check for captures
    // don't check for king in check
}

