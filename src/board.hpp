
#pragma once

#include <vector>

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
    int rank;
    fileVals file;
};

struct Board {
    // begin default game
    Board(); 
    // maybe useful for testing or chess960
    Board(std::vector<std::vector<pieceTypes>> board, bool isWhiteTurn = true, 
            int movesWithoutCapture = 0, bool pawnJumped = false, 
            BoardSquare pawnJumpedSquare = BoardSquare(), bool inCheck = false); 
    // creates a new board for a move; defined in inCheck.cpp
    Board(Board originalBoard, BoardSquare pos1, BoardSquare pos2);

    pieceTypes getPiece(int rank, int file);
    pieceTypes getPiece(BoardSquare square);

    std::vector<std::vector<pieceTypes>> board;
    bool isWhiteTurn;
    int movesWithoutCapture; // 50 move rule
    bool inCheck;
    bool pawnJumped;
    BoardSquare pawnJumpedSquare;
};

