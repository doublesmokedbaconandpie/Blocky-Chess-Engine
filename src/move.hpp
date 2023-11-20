#pragma once

#include <string>

#include "types.hpp"

#define Square uint8_t

class BoardMove {
    public:
        BoardMove(Square square1, Square square2, pieceTypes promotePiece = EmptyPiece);
        BoardMove(std::string input, bool isWhiteTurn);
        std::string toStr() const;
        
        Square getSquare1() const;
        Square getSquare2() const;
        pieceTypes getPromotePiece() const;

        operator bool() const;
    private: 
        pieceTypes toPromotePiece(int integer) const;
        int toInt(pieceTypes piece) const;

        std::string toStr(Square square) const;
        std::string toStr(pieceTypes piece) const;
        Square fromStr(std::string input);

        uint16_t data;

    friend std::ostream& operator<<(std::ostream& os, const BoardMove& target);
    friend bool operator==(const BoardMove& lhs, const BoardMove& rhs);
    friend bool operator!=(const BoardMove& lhs, const BoardMove& rhs);
    friend bool operator<(const BoardMove& lhs, const BoardMove& rhs);
};
