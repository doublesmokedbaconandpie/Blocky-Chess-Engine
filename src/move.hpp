#pragma once

#include <string>

#include "types.hpp"

struct BoardSquare {
    BoardSquare(): rank(-1), file(nullFile) {};
    BoardSquare(int a_rank, fileVals a_file): rank(a_rank), file(a_file) {};
    BoardSquare(int a_rank, int a_file): rank(a_rank), file(fileVals(a_file)) {};
    BoardSquare(std::string input);
    BoardSquare(int square); // from square
    std::string toStr();
    int toSquare();
    bool isValid() const;

    friend bool operator==(const BoardSquare& lhs, const BoardSquare& rhs);
    friend bool operator!=(const BoardSquare& lhs, const BoardSquare& rhs);
    friend bool operator<(const BoardSquare& lhs, const BoardSquare& rhs);
    friend std::ostream& operator<<(std::ostream& os, const BoardSquare& target);
    int rank;
    fileVals file;
};

struct BoardMove {
    BoardSquare pos1;
    BoardSquare pos2;
    pieceTypes promotionPiece;
    BoardMove(BoardSquare a_pos1 = BoardSquare(), BoardSquare a_pos2 = BoardSquare(), pieceTypes a_promotionPiece = nullPiece): 
        pos1(a_pos1), pos2(a_pos2), promotionPiece(a_promotionPiece) {}; 
    BoardMove(std::string input, bool isWhiteTurn);
    std::string toStr() const;
    bool isValid() const;

    friend std::ostream& operator<<(std::ostream& os, const BoardMove& target);
    friend bool operator==(const BoardMove& lhs, const BoardMove& rhs);
    friend bool operator<(const BoardMove& lhs, const BoardMove& rhs);
};