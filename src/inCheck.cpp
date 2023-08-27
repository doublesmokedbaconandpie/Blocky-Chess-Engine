#include "inCheck.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

bool isFriendlyPiece(Board& currBoard, BoardSquare targetSquare) {
    int target = currBoard.getPiece(targetSquare);
    if(currBoard.isWhiteTurn) {
        return target >= WKing && target <= WPawn;
    }
    else {
        return target >= BKing && target <= BPawn;
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
    return nullPiece;
}

bool checkDiagAttackers(Board& currBoard, BoardSquare originSquare) {
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

bool checkStraightAttackers(Board& currBoard, BoardSquare originSquare) {
    std::vector<pieceTypes> possibleAttackers;
    if (currBoard.isWhiteTurn) {
        possibleAttackers.push_back(BQueen);
        possibleAttackers.push_back(BRook);
    }
    else {
        possibleAttackers.push_back(WQueen);
        possibleAttackers.push_back(WRook);
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



bool checkKnightAttackers(Board& currBoard, BoardSquare originSquare) {
    pieceTypes enemyKnight = currBoard.isWhiteTurn ? BKnight : WKnight;

    std::vector<pieceTypes> knightSquares; 
    int rank = originSquare.rank;
    int file = originSquare.file;
    knightSquares.push_back(currBoard.getPiece(rank - 2, file - 1));
    knightSquares.push_back(currBoard.getPiece(rank - 2, file + 1));
    knightSquares.push_back(currBoard.getPiece(rank + 2, file - 1));
    knightSquares.push_back(currBoard.getPiece(rank + 2, file + 1));
    knightSquares.push_back(currBoard.getPiece(rank - 1, file - 2));
    knightSquares.push_back(currBoard.getPiece(rank + 1, file - 2));
    knightSquares.push_back(currBoard.getPiece(rank - 1, file + 2));
    knightSquares.push_back(currBoard.getPiece(rank + 1, file + 2));

    for (pieceTypes piece: knightSquares) {
        if (piece == enemyKnight) {
            return true;
        }
    }
    return false;
}

bool checkPawnAttackers(Board& currBoard, BoardSquare originSquare) {
    pieceTypes enemyPawn = currBoard.isWhiteTurn ? BPawn : WPawn;
    int direction = currBoard.isWhiteTurn ? -1 : 1;

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
        if (piece == enemyPawn) {
            return true;
        }
    }
    return false;
}

bool checkKingAttackers(Board& currBoard, BoardSquare originSquare) {
    pieceTypes enemyKing = currBoard.isWhiteTurn ? BKing : WKing;

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
        if (piece == enemyKing) {
            return true;
        }
    }
    return false;
}


bool currKingInAttack(Board currBoard) {
    pieceTypes allyKing = currBoard.isWhiteTurn ? WKing : BKing;

    BoardSquare currKingSquare = BoardSquare();
    for (int rank = 0; rank <= 7; rank++) {
        if (currKingSquare != BoardSquare()) {
            break;
        }
        for (int file = A; file <= H; file++) {
            pieceTypes currPiece = currBoard.getPiece(rank, file);
            if (currPiece == allyKing) {
                currKingSquare = BoardSquare(rank, file);
                break;
            }
        }
    }
    
    return checkDiagAttackers(currBoard, currKingSquare) 
        || checkStraightAttackers(currBoard, currKingSquare)
        || checkKnightAttackers(currBoard, currKingSquare)
        || checkPawnAttackers(currBoard, currKingSquare)
        || checkKingAttackers(currBoard, currKingSquare);
}


bool notInRange(int var) {return var < 0 || var > 7;}
bool Board::makeMove(BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece) {
    if (notInRange(pos1.rank) || notInRange(pos1.file) || notInRange(pos2.file) || notInRange(pos2.rank)) {
        this->isIllegalPos = true;
        return false;
    }

    // allies haven't made a move yet
    pieceTypes allyKing = this->isWhiteTurn ? WKing : BKing;
    pieceTypes allyRook = this->isWhiteTurn ? WRook : BRook;
    pieceTypes allyPawn = this->isWhiteTurn ? WPawn : BPawn;
    int pawnJumpDirection = this->isWhiteTurn ? -2 : 2;
    int promotionRank = this->isWhiteTurn ? 0 : 7;
    
    pieceTypes originPiece = this->getPiece(pos1);
    pieceTypes targetPiece = this->getPiece(pos2);

    this->moveHistory.push_back(BoardState(
        BoardMove(pos1, pos2, promotionPiece),
        originPiece,
        targetPiece,
        this->castlingRights,
        this->pawnJumpedSquare,
        this->fiftyMoveRule,
        this->materialDifference
    ));

    BoardSquare oldPawnJumpedSquare = this->pawnJumpedSquare;

    this->fiftyMoveRule += 1;
    this->setPiece(pos1, EmptyPiece); // origin square should be cleared in all situations

    // castling
    // doesn't check for emptiness between rook and king
    if (originPiece == allyKing && (this->castlingRights & castleRightsBit(pos2))) {
        int kingFileDirection = pos2.file > pos1.file ? 1 : -1;
        fileVals rookFile = kingFileDirection == 1 ? H : A;
        this->setPiece(pos2, allyKing);
        this->setPiece(pos1.rank, pos1.file + kingFileDirection, allyRook);
        this->setPiece(pos1.rank, rookFile, EmptyPiece);
        this->castlingRights &= allyKing == WKing ? B_Castle : W_Castle;
    }
    else if (originPiece == allyKing) {
        this->setPiece(pos2, originPiece);
        this->castlingRights &= allyKing == WKing ? B_Castle : W_Castle;
    }
    else if (originPiece == allyRook) {
        this->setPiece(pos2, originPiece);
        this->castlingRights &= pos1 == BoardSquare("h1") ? NOT_W_OO  : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("a1") ? NOT_W_OOO : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("h8") ? NOT_B_OO  : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("a8") ? NOT_B_OOO : All_Castle;
    }
    // jumping pawn
    else if (originPiece == allyPawn && pos2.rank == pos1.rank + pawnJumpDirection) { 
        // doesn't check if pawn's original position is rank 2
        int behindDirection = this->isWhiteTurn ? 1 : -1;
        this->setPiece(pos2, originPiece);
        this->pawnJumpedSquare = BoardSquare(pos2.rank + behindDirection, pos2.file);
        this->fiftyMoveRule = 0;
    }
    // promoting pawn
    else if (originPiece == allyPawn && pos2.rank == promotionRank) {
        this->setPiece(pos2, promotionPiece);
        this->fiftyMoveRule = 0;

        //updates material score of the board on promotion
        if(this->isWhiteTurn) {
            this->materialDifference += pieceValues.at(promotionPiece) - 1;
        }
        else {
            this->materialDifference += pieceValues.at(promotionPiece) + 1;
        }
    }
    // en passant 
    else if (originPiece == allyPawn && pos2 == this->pawnJumpedSquare) {
        this->setPiece(pos1.rank, pos2.file, EmptyPiece);
        this->setPiece(pos2, originPiece);
        this->fiftyMoveRule = 0;

        if(this->isWhiteTurn)
            materialDifference += 1;
        else
            materialDifference -= 1;

    }
    else {
        this->setPiece(pos2, originPiece);
        if (targetPiece != EmptyPiece || originPiece == allyPawn) {
            this->fiftyMoveRule = 0;
        }
    }

    this->materialDifference -= pieceValues.at(targetPiece);
    this->pawnJumpedSquare = this->pawnJumpedSquare == oldPawnJumpedSquare ? BoardSquare() : this->pawnJumpedSquare;
    this->isIllegalPos = currKingInAttack(*this);
    this->isWhiteTurn = !this->isWhiteTurn;  // after finalizing move logic, now switch turns
    return true;
}

bool Board::makeMove(BoardMove move) {
    return this->makeMove(move.pos1, move.pos2, move.promotionPiece);
}

bool Board::undoMove() {
    if (this->moveHistory.size() == 0) {
        return false;
    }
    BoardState prev = moveHistory.back();

    pieceTypes prevKing = this->isWhiteTurn ? BKing : WKing;
    pieceTypes prevRook = this->isWhiteTurn ? BRook : WRook;
    pieceTypes prevPawn = this->isWhiteTurn ? BPawn : WPawn;

    this->setPiece(prev.move.pos1, prev.originPiece);
    this->setPiece(prev.move.pos2, prev.targetPiece);

    // castling
    if (prev.originPiece == prevKing && (prev.castlingRights & castleRightsBit(prev.move.pos2)) ) {
        int kingFileDirection = prev.move.pos2.file > prev.move.pos1.file ? 1 : -1;
        fileVals rookFile = kingFileDirection == 1 ? H : A;
        this->setPiece(prev.move.pos1.rank, prev.move.pos1.file + kingFileDirection, EmptyPiece);
        this->setPiece(prev.move.pos1.rank, rookFile, prevRook);
    }
    // en passant
    else if (prev.originPiece == prevPawn && prev.move.pos2 == prev.pawnJumpedSquare) {
        pieceTypes prevJumpedPawn = prevPawn == BPawn ? WPawn : BPawn;
        this->setPiece(prev.move.pos1.rank, prev.move.pos2.file, prevJumpedPawn);
    }

    this->isWhiteTurn = !this->isWhiteTurn;
    this->castlingRights = prev.castlingRights;
    this->pawnJumpedSquare = prev.pawnJumpedSquare;
    this->fiftyMoveRule = prev.fiftyMoveRule;
    this->materialDifference = prev.materialDifference;
    this->isIllegalPos = false;

    this->moveHistory.pop_back();
    return true;
}