
#include "board.hpp"

#include <iostream>

// BoardSquare

bool operator==(const BoardSquare& lhs, const BoardSquare& rhs) {
        return (lhs.rank == rhs.rank) && (lhs.file == rhs.file);
}

bool operator!=(const BoardSquare& lhs, const BoardSquare& rhs) {
    return (lhs.rank != rhs.rank) || (lhs.file != rhs.file);
}

bool operator<(const BoardSquare& lhs, const BoardSquare& rhs) {
    if (lhs.rank != rhs.rank) {
        return lhs.rank < rhs.rank;
    }
    if (lhs.file != rhs.file) {
        return lhs.file < rhs.file;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const BoardSquare& target) {
        os << "Rank: " << target.rank << " File: " << target.file << ' ';
        return os;
}

// BoardMove

std::ostream& operator<<(std::ostream& os, const BoardMove& target) {
    os <<  "Pos1[" << target.pos1 << "] Pos2[" << target.pos2 << "] Promote: " << target.promotionPiece;
    return os;
}

bool operator==(const BoardMove& lhs, const BoardMove& rhs) {
    return (lhs.pos1 == rhs.pos1) && (lhs.pos2 == rhs.pos2) && (lhs.promotionPiece == rhs.promotionPiece);
}

bool operator<(const BoardMove& lhs, const BoardMove& rhs) {
    if (lhs.pos1 != rhs.pos1) {
        return lhs.pos1 < rhs.pos1;
    }
    if (lhs.pos2 != rhs.pos2) {
        return lhs.pos2 < rhs.pos2;
    }
    return false;
}

// Board

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
    this->movesSincePawnMovedOrCapture = 0;
    this->pawnJumped = false;
    this->pawnJumpedSquare = BoardSquare();
    this->isIllegalPos = false;
}

Board::Board(std::vector<std::vector<pieceTypes>> board, bool isWhiteTurn, 
                int movesSincePawnMovedOrCapture, bool pawnJumped, 
                BoardSquare pawnJumpedSquare, bool isIllegalPos) {
    this->board = board;
    this->isWhiteTurn = isWhiteTurn;
    this-> movesSincePawnMovedOrCapture = movesSincePawnMovedOrCapture;
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
            switch (j)
            {
            case WPawn:
            case WPawnJumped:
                os << "WP";
                break;
            case WKnight:
                os << "WN";
                break;
            case WBishop:
                os << "WB";
                break;
            case WRook:
            case WRookUnmoved:
                os << "WR";
                break;
            case WQueen:
                os << "WQ";
                break;
            case WKing:
            case WKingUnmoved:
                os << "WK";
                break;
            case BPawn:
            case BPawnJumped:
                os << "BP";
                break;
            case BKnight:
                os << "BN";
                break;
            case BBishop:
                os << "BB";
                break;
            case BRook:
            case BRookUnmoved:
                os << "BR";
                break;
            case BQueen:
                os << "BQ";
                break;
            case BKing:
            case BKingUnmoved:
                os << "BK";
                break;
            default:
                os << "  ";
            }
            os << ',';
        }
        os<< "],\n";
    }
    os << "\n";
    os << target.isIllegalPos << " ";
    os << target.isWhiteTurn << " ";
    os << target.movesSincePawnMovedOrCapture << "\n";
    return os;
}