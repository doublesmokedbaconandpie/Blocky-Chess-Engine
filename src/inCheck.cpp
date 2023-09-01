#include "inCheck.hpp"
#include "bitboard.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

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

bool currKingInAttack(Board& board) {
    pieceTypes allyKing = board.isWhiteTurn ? WKing : BKing;
    int kingSquare = leadingBit(board.pieceSets[allyKing]);
    assert(kingSquare != -1);

    uint64_t allPieces = board.pieceSets[WHITE_PIECES] | board.pieceSets[BLACK_PIECES];

    uint64_t enemyKings   = board.isWhiteTurn ? board.pieceSets[BKing]   : board.pieceSets[WKing];
    uint64_t enemyQueens  = board.isWhiteTurn ? board.pieceSets[BQueen]  : board.pieceSets[WQueen];
    uint64_t enemyBishops = board.isWhiteTurn ? board.pieceSets[BBishop] : board.pieceSets[WBishop];
    uint64_t enemyRooks   = board.isWhiteTurn ? board.pieceSets[BRook]   : board.pieceSets[WRook];
    uint64_t enemyKnights = board.isWhiteTurn ? board.pieceSets[BKnight] : board.pieceSets[WKnight];
    uint64_t enemyPawns   = board.isWhiteTurn ? board.pieceSets[BPawn]   : board.pieceSets[WPawn];

    return diagAttackers(kingSquare, allPieces, enemyQueens | enemyBishops)
        || straightAttackers(kingSquare, allPieces, enemyQueens | enemyRooks)
        || knightAttackers(kingSquare, enemyKnights)
        || pawnAttackers(kingSquare, enemyPawns, board.isWhiteTurn)
        || kingAttackers(kingSquare, enemyKings);
}

bool notInRange(int var) {return var < 0 || var > 7;}
void Board::makeMove(BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece) {
    if (notInRange(pos1.rank) || notInRange(pos1.file) || notInRange(pos2.file) || notInRange(pos2.rank)) {
        this->isIllegalPos = true;
        return;
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

    this->fiftyMoveRule++;

    this->setPiece(pos1, EmptyPiece); // origin square should be cleared in all situations
    this->setPiece(pos2, originPiece); // pretty much all possible moves translates the original piece to pos 2

    // castling
    // doesn't check for emptiness between rook and king
    if (originPiece == allyKing && (this->castlingRights & castleRightsBit(pos2))) {
        int kingFileDirection = pos2.file > pos1.file ? 1 : -1;
        fileVals rookFile = kingFileDirection == 1 ? H : A;
        this->setPiece(pos1.rank, pos1.file + kingFileDirection, allyRook);
        this->setPiece(pos1.rank, rookFile, EmptyPiece);
        this->castlingRights &= allyKing == WKing ? B_Castle : W_Castle;
    }
    else if (originPiece == allyKing) {
        this->castlingRights &= allyKing == WKing ? B_Castle : W_Castle;
    }
    else if (originPiece == allyRook) {
        this->castlingRights &= pos1 == BoardSquare("h1") ? NOT_W_OO  : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("a1") ? NOT_W_OOO : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("h8") ? NOT_B_OO  : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("a8") ? NOT_B_OOO : All_Castle;
    }
    // jumping pawn
    else if (originPiece == allyPawn && pos2.rank == pos1.rank + pawnJumpDirection) { 
        // doesn't check if pawn's original position is rank 2
        int behindDirection = this->isWhiteTurn ? 1 : -1;
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
        this->fiftyMoveRule = 0;

        if(this->isWhiteTurn)
            materialDifference++;
        else
            materialDifference--;

    }
    else {
        if (targetPiece != EmptyPiece || originPiece == allyPawn) {
            this->fiftyMoveRule = 0;
        }
    }

    this->materialDifference -= pieceValues.at(targetPiece); //updates the material score of the board on capture
    this->pawnJumpedSquare = this->pawnJumpedSquare == oldPawnJumpedSquare ? BoardSquare() : this->pawnJumpedSquare;

    this->isIllegalPos = currKingInAttack(*this);
    // after finalizing move logic, now switch turns
    this->isWhiteTurn = !this->isWhiteTurn; 
}

void Board::makeMove(BoardMove move) {
    this->makeMove(move.pos1, move.pos2, move.promotionPiece);
}


void Board::undoMove() {
    if (this->moveHistory.size() == 0) {
        return;
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
}
