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

#pragma once

#include <string>

#include "utils/types.hpp"

constexpr Square NULLSQUARE = 0xFF;
constexpr uint16_t NULLMOVE = 0xFFFF;

class Move {
    public:
        Move() : data(NULLMOVE) {};
        Move(Square square1, Square square2, pieceTypes promotePiece = EmptyPiece);
        Move(std::string input, bool isWhiteTurn);
        std::string toStr() const;

        Square sqr1() const;
        Square sqr2() const;
        pieceTypes getPromotePiece() const;

        operator bool() const;
        bool operator==(const Move& rhs) const;
        bool operator!=(const Move& rhs) const;
        std::ostream& operator<<(std::ostream& os) const;
    private:
        pieceTypes toPromotePiece(int integer) const;
        int toInt(pieceTypes piece) const;
        std::string toStr(pieceTypes piece) const;

        uint16_t data{};
};

Square toSquare(int rank, int file);
Square toSquare(std::string input);
std::string squareToStr(Square square);