
#pragma once

#include <vector>
#include <iostream>

enum fileVals {nullFile = -1, A, B, C, D, E, F, G, H};
enum pieceTypes {nullPiece = -1, EmptyPiece,
                WKing, WQueen, WBishop, WKnight, WRook, WPawn, 
                WPawnJumped, WKingUnmoved, WRookUnmoved,
                BKing, BQueen, BBishop, BKnight, BRook, BPawn,
                BPawnJumped, BKingUnmoved, BRookUnmoved,};

struct BoardSquare {
    BoardSquare(): rank(-1), file(nullFile) {};
    BoardSquare(int rankVal, fileVals fileVal): rank(rankVal), file(fileVal) {};
    BoardSquare(int rankVal, int fileVal): rank(rankVal), file((fileVals)fileVal) {};
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
    friend std::ostream& operator<<(std::ostream& os, const BoardMove& target);
    friend bool operator==(const BoardMove& lhs, const BoardMove& rhs);
    friend bool operator<(const BoardMove& lhs, const BoardMove& rhs);
};

struct Board {
    // begin default game
    Board(); 
    // maybe useful for testing or chess960
    Board(std::vector<std::vector<pieceTypes>> board, bool isWhiteTurn = true, 
            int movesWithoutCapture = 0, bool pawnJumped = false, 
            BoardSquare pawnJumpedSquare = BoardSquare(), bool isIllegalPos = false); 
    // creates a new board for a move; defined in inCheck.cpp
    Board(Board& originalBoard, BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece = nullPiece);
    Board(Board& originalBoard, BoardMove move);
    
    friend bool operator==(const Board& lhs, const Board& rhs);
    friend bool operator<(const Board& lhs, const Board& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Board& target);

    pieceTypes getPiece(int rank, int file) const;
    pieceTypes getPiece(BoardSquare square) const;
    bool setPiece(int rank, int file, pieceTypes piece);
    bool setPiece(BoardSquare square, pieceTypes piece);

    std::vector<std::vector<pieceTypes>> board;
    bool isWhiteTurn;
    int movesSincePawnMoved; // 50 move rule
    bool isIllegalPos;
    bool pawnJumped;
    BoardSquare pawnJumpedSquare;
};

