/*
* Blocky, a UCI chess engine
* Copyright (C) 2023-2024, Kevin Nguyen
*
* Blocky is free software; you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
*
* Blocky is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along with this program; 
* if not, see <https://www.gnu.org/licenses>.
*/


#pragma once

#include <array>
#include <cstdint>
#include <unordered_map>

constexpr int BOARD_SIZE = 64;
constexpr int NUM_FILES = 8;
constexpr int NUM_RANKS = 8;
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
                WHITE_PIECES, BLACK_PIECES,
                
                KING = WKing, KNIGHT = WKnight, BISHOP = WBishop, ROOK = WRook, QUEEN = WQueen, PAWN = WPawn
                };

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

// internally uses a fixed size array to avoid memory reallocations, like what can happen in std::vector
// there is no bounds checking, so be careful with push and pop backs
template<typename T, std::size_t N>
class FixedVector {
    public:
        // writing
        constexpr T& operator[](std::size_t index) {
            return this->container[index];
        }
        // reading
        constexpr const T& operator[](std::size_t index) const {
            return this->container[index];
        }
        constexpr void push_back(const T& entry) {
            this->pointer++;
            this->container[this->pointer] = entry;
        }
        constexpr void pop_back() {
            // there is no need to null initialize the old memory since push_back is the only valid way to access it
            this->pointer--;
        }
        constexpr T back() {
            return this->container[this->pointer];
        }
        constexpr auto begin() const {
            return this->container.begin();
        }
        constexpr auto end() const {
            return this->container.begin() + this->pointer + 1;
        }
        constexpr auto size() const {
            return this->pointer + 1;
        }
        constexpr auto clear() {
            // there is no need to null initialize the old memory since push_back is the only valid way to access it
            this->pointer = -1;
        }
        constexpr bool operator==(const FixedVector<T, N>& rhs) const {
            if (this->size() != rhs.size()) {
                return false;
            }
            for (std::size_t i = 0; i < this->size(); ++i) {
                if (this->container[i] != rhs[i]) {
                    return false;
                }
            }
            return true;
        }
    private:
        std::array<T, N> container{};
        std::size_t pointer = -1;
};
