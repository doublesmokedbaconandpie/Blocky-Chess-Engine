#include <string>
#include <sstream>
#include <array>

#include "move.hpp"

// BoardSquare

BoardSquare::BoardSquare(std::string input) {
    if (input == "-") {
        *this = BoardSquare();
        return;
    }
    this->file = fileVals(input.at(0) - 'a');
    this->rank = 8 - int(input.at(1) - '1') - 1;
}

int BoardSquare::toSquare() {
    return this->rank * 8 + this->file;
}

std::string BoardSquare::toStr() {
    std::ostringstream move;
    move << *this;
    return move.str();
}

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
    if (target == BoardSquare()) {
        os << "-";
        return os;
    }
    if (target.rank < 0 || target.rank > 7 || target.file < A || target.file > H) {
        os << target.file << ' ' << target.rank << ' ';
        return os;    
    }
    std::array<char, 8> fileRep = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    std::array<char, 8> rankRep = {'8', '7', '6', '5', '4', '3', '2', '1'};
    os << fileRep.at(target.file) << rankRep.at(target.rank);
    return os;
}

// BoardMove

BoardMove::BoardMove(std::string input, bool isWhiteTurn) {
    pieceTypes allyQueen = isWhiteTurn ? WQueen : BQueen;
    pieceTypes allyBishop = isWhiteTurn ? WBishop : BBishop;
    pieceTypes allyKnight = isWhiteTurn ? WKnight : BKnight;
    pieceTypes allyRook = isWhiteTurn ? WRook : BRook;
    
    this->pos1 = BoardSquare(input.substr(0, 2));
    this->pos2 = BoardSquare(input.substr(2, 2));

    if (input.length() == 5) {
        switch (input.at(4)) {
            case 'q':
                this->promotionPiece = allyQueen;
                break;
            case 'n':
                this->promotionPiece = allyKnight;
                break;
            case 'b':
                this->promotionPiece = allyBishop;
                break;
            case 'r':
                this->promotionPiece = allyRook;
                break;
            default:
                this->promotionPiece = nullPiece;
        }
    }
}

std::string BoardMove::toStr() {
    char pp;
    switch (this->promotionPiece) {
        case WQueen:
        case BQueen:
            pp = 'q';
            break;
        case WBishop:
        case BBishop:
            pp = 'b';
            break;
        case WRook:
        case BRook:
            pp = 'r';
            break;
        case WKnight:
        case BKnight:
            pp = 'n';
            break;
        default:
            pp = ' ';
    }
    std::ostringstream move;
    move << this->pos1;
    move << this->pos2;
    move << pp;
    return move.str();
}

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
