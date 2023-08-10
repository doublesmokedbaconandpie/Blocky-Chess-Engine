
#include "board.hpp"

Board::Board() {
    this->board = {
        {BRookUnmoved, BKnight, BBishop, BQueen, BKingUnmoved, BBishop, BKnight, BRookUnmoved},
        {BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn},
        {WRookUnmoved, WKnight, WBishop, WQueen, WKingUnmoved, WBishop, WKnight, WRookUnmoved}
    };
    this->isWhiteTurn = true;
    this->movesWithoutCapture = 0;
    this->pawnJumped = false;
    this->pawnJumpedSquare = BoardSquare();
    this->isIllegalPos = false;
}

Board::Board(std::vector<std::vector<pieceTypes>> board, bool isWhiteTurn, 
                int movesWithoutCapture, bool pawnJumped, 
                BoardSquare pawnJumpedSquare, bool isIllegalPos) {
    this->board = board;
    this->isWhiteTurn = isWhiteTurn;
    this-> movesWithoutCapture = movesWithoutCapture;
    this->pawnJumped = pawnJumped;
    this->pawnJumpedSquare = pawnJumpedSquare;
    this->isIllegalPos = isIllegalPos;
}
pieceTypes Board::getPiece(int rank, int file) {
    if (rank < 0 || rank > 7 || file < A || file > H) {
        return nullPiece;
    }
    return this->board.at(rank).at(file);
}

pieceTypes Board::getPiece(BoardSquare square)  {
    return this->getPiece(square.rank, square.file);
}

bool Board::setPiece(int rank, int file, pieceTypes piece) {
    if (rank < 0 || rank > 7 || file < A || file > H) {
        return false;
    }
    this->board.at(rank).at(file) = piece;
    return true;
}

bool Board::setPiece(BoardSquare square, pieceTypes piece) {
    return this->setPiece(square.rank, square.file, piece);
}