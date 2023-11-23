#pragma once

#include <array>
#include <unordered_map>

#define Square uint8_t

constexpr int BOARD_SIZE = 64;
constexpr int NUM_BITBOARDS = 14;
constexpr int NUM_PIECE_TYPES = 12;

enum fileVals {nullFile = -1, A, B, C, D, E, F, G, H};

enum pieceTypes {nullPiece = -2, EmptyPiece,
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

enum NodeTypes {
    ROOT, PV, NOTPV, NMP
};

// indices are equal to the enumerated pieceTypes
constexpr std::array<int, 13> pieceValues {
    0, 9, 3, 3, 5, 1,
    0, 9, 3, 3, 5, 1,};

inline bool isWhitePiece(pieceTypes piece) {
    return piece >= WKing && piece <= WPawn;
}

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
