
#pragma once

#include <vector>

enum fileVals {nullFile = -1, A, B, C, D, E, F, G, H};
enum pieceTypes {WKing = 1, WQueen, WBishop, WKnight, WRook, WPawn, 
                WPawnJumped, WKingUnmoved, WRookUnmoved,
                BKing, BQueen, BBishop, BKnight, BRook, BPawn,
                BPawnJumped, BKingUnmoved, BRookUnmoved,};

struct BoardSquare {
    BoardSquare(): file(nullFile), rank(-1) {};
    BoardSquare(fileVals fileVal, int rankVal): file(fileVal), rank(rankVal) {};
    BoardSquare(int fileVal, int rankVal): file((fileVals)fileVal), rank(rankVal) {};
    fileVals file;
    int rank;
};

struct Board {
    Board(); // begin default game
    Board(std::vector<std::vector<int>> board, bool isWhiteTurn = true, 
            int movesWithoutCapture = 0, bool pawnJumped = false, 
            BoardSquare pawnJumpedSquare = BoardSquare(), bool inCheck = false); // maybe useful for testing or chess960
    Board(Board originalBoard, BoardSquare pos1, BoardSquare pos2); // creates a new board for a move 

    int getPiece(int rank, int file) {
        return this->board.at(rank).at(file);
    };

    std::vector<std::vector<int>> board;
    bool isWhiteTurn;
    int movesWithoutCapture; // 50 move rule
    bool inCheck;
    bool pawnJumped;
    BoardSquare pawnJumpedSquare;
};


