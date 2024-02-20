#include <cassert>

#include "pieceSets.hpp"

auto PieceSets::get(pieceTypes piece) const -> uint64_t {
    switch (piece)
    {
    case KING:
        return this->data[WKing] | this->data[BKing];
    case KNIGHT:
        return this->data[WKnight] | this->data[BKnight];
    case BISHOP:
        return this->data[WBishop] | this->data[BBishop];
    case ROOK:
        return this->data[WRook] | this->data[BRook];
    case QUEEN:
        return this->data[WQueen] | this->data[BQueen];
    case PAWN:
        return this->data[WPawn] | this->data[BPawn];
    case ALL:
        return this->data[WHITE_PIECES] | this->data[BLACK_PIECES];
    default:
        assert(false);
    }
}

auto PieceSets::get(pieceTypes piece, bool isWhite) const -> const uint64_t& {
    switch (piece)
    {
    case KING:
        return isWhite ? this->data[WKing] : this->data[BKing];
    case KNIGHT:
        return isWhite ? this->data[WKnight] : this->data[BKnight];
    case BISHOP:
        return isWhite ? this->data[WBishop] : this->data[BBishop];
    case ROOK:
        return isWhite ? this->data[WRook] : this->data[BRook];
    case QUEEN:
        return isWhite ? this->data[WQueen] : this->data[BQueen];
    case PAWN:
        return isWhite ? this->data[WPawn] : this->data[BPawn];
    case ALL:
        return isWhite ? this->data[WHITE_PIECES] : this->data[BLACK_PIECES];
    default:
        assert(false);
    }
}