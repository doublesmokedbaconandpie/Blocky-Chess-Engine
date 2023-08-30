#pragma once

#include <map>

constexpr int BOARD_SIZE = 64;
constexpr int NUM_BITBOARDS = 15;

enum fileVals {nullFile = -1, A, B, C, D, E, F, G, H};

enum pieceTypes {nullPiece = -1, EmptyPiece,
                WKing, WQueen, WBishop, WKnight, WRook, WPawn, 
                BKing, BQueen, BBishop, BKnight, BRook, BPawn,
                WHITE_PIECES, BLACK_PIECES};

enum castleRights {
    noCastle = 0, 
    W_OO = 1, 
    W_OOO = W_OO << 1,
    B_OO = W_OO << 2,
    B_OOO = W_OO << 3,

    K_Castle = W_OO | B_OO,
    Q_Castle = W_OOO | B_OOO,
    W_Castle = W_OO | W_OOO,
    B_Castle = B_OO | B_OOO,

    NOT_W_OO = B_Castle | W_OOO,
    NOT_W_OOO = B_Castle | W_OO,
    NOT_B_OO = W_Castle | B_OOO,
    NOT_B_OOO = W_Castle | B_OO,

    All_Castle = W_Castle | B_Castle,
};

static std::map<int, int> pieceValues = {
    {WPawn, 1},
    {BPawn, -1},
    {WBishop, 3}, 
    {BBishop, -3}, 
    {WKnight, 3}, 
    {BKnight, -3}, 
    {WRook, 5}, 
    {BRook, -5}, 
    {WQueen, 9},
    {BQueen, -9},
    {WKing, 1000}, //should not be used
    {BKing, -1000}, //should not be used
    {EmptyPiece, 0},
    {nullPiece, 0}
};


constexpr inline castleRights operator&(castleRights lhs, castleRights rhs) {
    return static_cast<castleRights>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

constexpr inline castleRights operator^(castleRights lhs, castleRights rhs) {
    return static_cast<castleRights>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
}

constexpr inline castleRights& operator&=(castleRights& lhs, castleRights rhs) {
    return lhs = lhs & rhs;
}

constexpr inline castleRights& operator^=(castleRights& lhs, castleRights rhs) {
    return lhs = lhs ^ rhs;
}
