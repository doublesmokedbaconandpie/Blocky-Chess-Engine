#pragma once

#include <array>
#include <cstdint>
#include <unordered_map>

constexpr int BOARD_SIZE = 64;
constexpr int NUM_BITBOARDS = 14;
constexpr int NUM_COLORED_PIECES = 12;
constexpr int NUM_PIECES = 6;
constexpr int MAX_MOVES = 256;
constexpr int MAX_PLY = 250;

using Square = uint8_t;
using PieceSets = std::array<uint64_t, NUM_BITBOARDS>;
using HistoryTable = std::array<std::array<uint64_t, BOARD_SIZE>, BOARD_SIZE>;

enum pieceTypes {EmptyPiece = -1,
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

enum EvalType {
    UPPER, LOWER, EXACT, NONE
};

enum NodeTypes {
    ROOT, PV, NOTPV, NMP
};

// indices are equal to the enumerated pieceTypes
constexpr std::array<int, 13> pieceValues {
    0, 9, 3, 3, 5, 1,
    0, 9, 3, 3, 5, 1,};

inline int getFile(Square square) {
    return square % 8;
}

inline int getRank(Square square) {
    return square / 8;
}

const std::unordered_map<pieceTypes, char> pieceToChar {
    {EmptyPiece, ' '},
    {WPawn, 'P'}, {WKnight, 'N'}, {WBishop, 'B'}, {WRook, 'R'}, {WQueen, 'Q'}, {WKing, 'K'}, 
    {BPawn, 'p'}, {BKnight, 'n'}, {BBishop, 'b'}, {BRook, 'r'}, {BQueen, 'q'}, {BKing, 'k'}, 
};

const std::unordered_map<char, pieceTypes> charToPiece {
    {'P', WPawn}, {'N', WKnight}, {'B', WBishop}, {'R', WRook}, {'Q', WQueen}, {'K', WKing}, 
    {'p', BPawn}, {'n', BKnight}, {'b', BBishop}, {'r', BRook}, {'q', BQueen}, {'k', BKing}, 
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
