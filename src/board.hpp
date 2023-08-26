
#pragma once

#include <array>
#include <iostream>
#include <string>

#include "types.hpp"

struct BoardSquare {
    BoardSquare(): rank(-1), file(nullFile) {};
    BoardSquare(int rankVal, fileVals fileVal): rank(rankVal), file(fileVal) {};
    BoardSquare(int rankVal, int fileVal): rank(rankVal), file((fileVals)fileVal) {};
    BoardSquare(std::string input);
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
    BoardMove(BoardSquare pos1 = BoardSquare(), BoardSquare pos2 = BoardSquare(), pieceTypes promotionPiece = nullPiece): 
        pos1(pos1), pos2(pos2), promotionPiece(promotionPiece) {}; 
    BoardMove(std::string input, bool isWhiteTurn);
    std::string toStr();

    friend std::ostream& operator<<(std::ostream& os, const BoardMove& target);
    friend bool operator==(const BoardMove& lhs, const BoardMove& rhs);
    friend bool operator<(const BoardMove& lhs, const BoardMove& rhs);
};

struct Board {
    Board(); // default game
    Board(std::array<pieceTypes, BOARD_SIZE> board, bool isWhiteTurn = true, 
            int movesWithoutCapture = 0, BoardSquare pawnJumpedSquare = BoardSquare(), 
            bool isIllegalPos = false, castleRights castlingRights = All_Castle, int materialDifference = 0); 
    Board(std::string fenStr);
    
    // defined in inCheck.cpp
    bool makeMove(BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece = nullPiece);
    bool makeMove(BoardMove move);
    bool undoMove();
    
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
    int fiftyMoveRule;
    bool isIllegalPos;
    BoardSquare pawnJumpedSquare;
    int materialDifference; //updates on capture or promotion, so the eval doesn't have to calculate for each board, positive is white advantage

    BoardMove previousMove = BoardMove();
    pieceTypes previousCapturedPiece = nullPiece;
};

castleRights castleRightsBit(BoardSquare finalKingPos);
