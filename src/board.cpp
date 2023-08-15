
#include "board.hpp"

#include <iostream>

bool operator==(const BoardSquare& lhs, const BoardSquare& rhs) {
        return (lhs.rank == rhs.rank) && (lhs.file == rhs.file);
}

bool operator!=(const BoardSquare& lhs, const BoardSquare& rhs) {
    return (lhs.rank != rhs.rank) || (lhs.file != rhs.file);
}

std::ostream& operator<<(std::ostream& os, const BoardSquare& target) {
        os << target.rank << target.file << ' ';
        return os;
}

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
    this->movesSincePawnMoved = 0;
    this->pawnJumped = false;
    this->pawnJumpedSquare = BoardSquare();
    this->isIllegalPos = false;
}

Board::Board(std::vector<std::vector<pieceTypes>> board, bool isWhiteTurn, 
                int movesSincePawnMoved, bool pawnJumped, 
                BoardSquare pawnJumpedSquare, bool isIllegalPos) {
    this->board = board;
    this->isWhiteTurn = isWhiteTurn;
    this-> movesSincePawnMoved = movesSincePawnMoved;
    this->pawnJumped = pawnJumped;
    this->pawnJumpedSquare = pawnJumpedSquare;
    this->isIllegalPos = isIllegalPos;
}
pieceTypes Board::getPiece(int rank, int file) const {
    if (rank < 0 || rank > 7 || file < A || file > H) {
        return nullPiece;
    }
    return this->board.at(rank).at(file);
}

pieceTypes Board::getPiece(BoardSquare square) const{
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

bool operator==(const Board& lhs, const Board& rhs) {
    return  (lhs.board == rhs.board);
}

bool operator<(const Board& lhs, const Board& rhs) {
    for (int rank = 0; rank <= 7; rank++) {
        for (int file = A; file <= H; file++) {
            pieceTypes lhsPiece = lhs.getPiece(rank, file);
            pieceTypes rhsPiece = rhs.getPiece(rank, file);
            if (lhsPiece != rhsPiece) {
                return lhsPiece < rhsPiece;
            }
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Board& target) {
    for (auto i: target.board) {
        os << "[";
        for (auto j: i) {
            os << j << ',';
        }
        os<< "],\n";
    }
    os << "\n";
    return os;
}