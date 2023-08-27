
#pragma once

#include <array>
#include <iostream>
#include <string>

#include "types.hpp"

struct BoardSquare {
    BoardSquare(): rank(-1), file(nullFile) {};
    BoardSquare(int a_rank, fileVals a_file): rank(a_rank), file(a_file) {};
    BoardSquare(int a_rank, int a_file): rank(a_rank), file(fileVals(a_file)) {};
    BoardSquare(std::string input);
    std::string toStr();

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
    std::string toStr();

    friend std::ostream& operator<<(std::ostream& os, const BoardMove& target);
    friend bool operator==(const BoardMove& lhs, const BoardMove& rhs);
    friend bool operator<(const BoardMove& lhs, const BoardMove& rhs);
};

struct Board {
    Board(); // default game
    Board(std::array<pieceTypes, BOARD_SIZE> a_board, bool a_isWhiteTurn = true, 
            int a_fiftyMoveRule = 0, BoardSquare a_pawnJumpedSquare = BoardSquare(), 
            bool a_isIllegalPos = false, castleRights a_castlingRights = All_Castle, int a_materialDifference = 0); 
    Board(std::string fenStr);
    std::string toFen();
    
    // defined in inCheck.cpp
    Board(Board& originalBoard, BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece = nullPiece);
    Board(Board& originalBoard, BoardMove move);
    
    friend bool operator==(const Board& lhs, const Board& rhs);
    friend bool operator<(const Board& lhs, const Board& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Board& target);

    pieceTypes getPiece(int rank, int file) const;
    pieceTypes getPiece(BoardSquare square) const;
    bool setPiece(int rank, int file, pieceTypes piece);
    bool setPiece(BoardSquare square, pieceTypes piece);

    std::array<pieceTypes, BOARD_SIZE> board = {EmptyPiece};
    bool isWhiteTurn;
    castleRights castlingRights; // bitwise castling rights tracker
    int fiftyMoveRule; // 50 move rule
    bool isIllegalPos;
    BoardSquare pawnJumpedSquare;
    int materialDifference; //updates on capture or promotion, so the eval doesn't have to calculate for each board, positive is white advantage
    //can reuse code in eval function if the position is not the starting position, otherwise defaults to 0
};

castleRights castleRightsBit(BoardSquare finalKingPos);
