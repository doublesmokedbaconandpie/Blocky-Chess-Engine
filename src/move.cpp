/*
* Blocky, a UCI chess engine
* Copyright (C) 2023-2024, Kevin Nguyen
*
* Blocky is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* Blocky is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, see <https://www.gnu.org/licenses>.
*/

#include <string>
#include <sstream>
#include <iostream>
#include <array>

#include "move.hpp"
#include "utils/types.hpp"

BoardMove::BoardMove(Square square1, Square square2, pieceTypes promotePiece) {
    data =  square1 | (square2 << 6) | (toInt(promotePiece) << 12);
}

BoardMove::BoardMove(std::string input, bool isWhiteTurn) {
    const pieceTypes allyQueen = isWhiteTurn ? WQueen : BQueen;
    const pieceTypes allyBishop = isWhiteTurn ? WBishop : BBishop;
    const pieceTypes allyKnight = isWhiteTurn ? WKnight : BKnight;
    const pieceTypes allyRook = isWhiteTurn ? WRook : BRook;
    
    const Square square1 = toSquare(input.substr(0, 2));
    const Square square2 = toSquare(input.substr(2, 2));
    pieceTypes promotePiece{};
    if (input.length() == 5) {
        switch (input.at(4)) {
            case 'q':
                promotePiece = allyQueen;
                break;
            case 'n':
                promotePiece = allyKnight;
                break;
            case 'b':
                promotePiece = allyBishop;
                break;
            case 'r':
                promotePiece = allyRook;
                break;
            default:
                promotePiece = EmptyPiece;
        }
    }
    data =  square1 | (square2 << 6) | (toInt(promotePiece) << 12);
}

std::string BoardMove::toStr() const {
    const auto square1 = squareToStr(sqr1());
    const auto square2 = squareToStr(sqr2());
    const auto promotePiece = toStr(getPromotePiece());
    return square1 + square2 + promotePiece;
}

uint8_t BoardMove::sqr1() const{
    constexpr uint16_t mask = 0x003F;
    return data & mask;
}

uint8_t BoardMove::sqr2() const{
    constexpr uint16_t mask = 0x003F;
    return (data >> 6) & mask;
}

pieceTypes BoardMove::getPromotePiece() const {
    constexpr uint16_t mask = 0x000F;
    const int piece = (data >> 12) & mask;
    return toPromotePiece(piece);
}

BoardMove::operator bool() const {
    return data != NULLMOVE;
}

bool operator==(const BoardMove& lhs, const BoardMove& rhs) {
    return lhs.data == rhs.data;
}

bool operator!=(const BoardMove& lhs, const BoardMove& rhs) {
    return lhs.data != rhs.data;
}

int BoardMove::toInt(pieceTypes piece) const {
    switch (piece)
    {
        case WQueen:
            return 1;
        case WBishop:
            return 2;
        case WKnight:
            return 3;
        case WRook:
            return 4;
        case BQueen:
            return 5;
        case BBishop:
            return 6;
        case BKnight:
            return 7;
        case BRook:
            return 8;
        default:
            return 0;
    }
}

pieceTypes BoardMove::toPromotePiece(int integer) const {
    switch (integer)
    {
        case 1:
            return WQueen;
        case 2:
            return WBishop;
        case 3:
            return WKnight;
        case 4:
            return WRook;
        case 5:
            return BQueen;
        case 6:
            return BBishop;
        case 7:
            return BKnight;
        case 8:
            return BRook;
        default:
            return EmptyPiece;
    }
}

std::string BoardMove::toStr(pieceTypes piece) const {
    switch (piece) {
            case WQueen:
            case BQueen:
                return "q";
            case WBishop:
            case BBishop:
                return "b";
            case WRook:
            case BRook:
                return "r";
            case WKnight:
            case BKnight:
                return "n";
            default:
                return "";
    }
}

std::ostream& operator<<(std::ostream& os, const BoardMove& target) {
    os << target.toStr();
    return os;
}

Square toSquare(std::string input) {
    if (input == "-") { // fen-style invalid square
        return NULLSQUARE;
    }
    const int file = input.at(0) - 'a';
    const int rank = 8 - int(input.at(1) - '1') - 1;
    return toSquare(rank, file);
}

Square toSquare(int rank, int file) {
    return rank * 8 + file;
}

std::string squareToStr(Square square) {
    if (square == NULLSQUARE) {
        return "-";
    }

    constexpr std::array<char, 8> fileRep = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    constexpr std::array<char, 8> rankRep = {'8', '7', '6', '5', '4', '3', '2', '1'};

    const int file = getFile(square);
    const int rank = getRank(square);
    return std::string(1, fileRep[file]) + std::string(1, rankRep[rank]);
}