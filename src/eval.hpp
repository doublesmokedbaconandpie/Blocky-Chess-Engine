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

#include "move.hpp"
#include "bitboard.hpp"
#include "types.hpp"

namespace Eval {

constexpr int TOTAL_PHASE = 24;
constexpr int PAWN_HASH_SIZE = 1024;

// evaluation scores
// contain both midgame and endgame scores
struct S {
    constexpr S() = default;
    constexpr S(int op, int eg) : opScore(op), egScore(eg) {};
    constexpr S& operator+=(const S& right) {
        this->opScore += right.opScore;
        this->egScore += right.egScore;
        return *this;
    }
    constexpr S& operator-=(const S& right) {
        this->opScore -= right.opScore;
        this->egScore -= right.egScore;
        return *this;
    }
    constexpr S operator+(const S& right) const {
        S result = *this;
        result += right;
        return result;
    };
    constexpr S operator-(const S& right) const {
        S result = *this;
        result -= right;
        return result;
    };
    constexpr S operator-() const {
        S result{};
        result -= *this;
        return result;
    };
    constexpr S operator*(const int& val) const {
        S result = *this;
        result.opScore *= val;
        result.egScore *= val;
        return result;
    };

    int opScore{};
    int egScore{};
};

struct PawnHashEntry {
    S score{};
    uint64_t key{};
};

class Info {
    public:
        Info() = default;
        int getRawEval(const PieceSets& pieceSets);
        void addPiece(Square square, pieceTypes piece);
        void removePiece(Square square, pieceTypes piece);
    private:
        const PawnHashEntry& getPawnInfo(const PieceSets& pieceSets);

        S score{};
        int phase{};
        std::array<PawnHashEntry, PAWN_HASH_SIZE> pawnHashTable{};
        uint64_t pawnKey{};
};

// other evaluation helper functions
S evalPawns(const PieceSets& pieceSets, bool isWhiteTurn);
int mopUpScore(const PieceSets& pieceSets, int eval);
int getPiecePhase(pieceTypes piece);
S getPSQTVal(Square square, pieceTypes currPiece);

// bitmasking tricks
bool isPassedPawn(Square pawn, uint64_t enemyPawns, bool isWhitePawn);

inline uint64_t getDoubledPawnsMask(uint64_t allyPawnSet, bool isWhitePawn) {
    const uint64_t forwardSquare = isWhitePawn ? allyPawnSet >> 8 : allyPawnSet << 8;
    return allyPawnSet & forwardSquare;
}


/*************
 * Evaluation Terms
**************/

// tables
constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 73,186), S( 62,112), S( 24, 63), S( -2, 34), S(-20, 10), S(  0, -1), S(  0,  0),};

constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(920,807), S(351,277), S(402,284), S(439,488), S( 73, 90), };

// misc terms
constexpr S doubledPawns{};

// piece square tables
constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[WKing] = {
    S( -27,-179), S( 200,-210), S( -19,  33), S(  71, -76), S(  29,  58), S( 832,-550), S(  54, -25), S(  13, -53),
    S( 792,-515), S(  36,   4), S(  -9,   5), S(   7,  55), S(  16, -14), S( -19,  42), S( 269,-160), S( 302,-102),
    S(  85, -35), S(  35,   3), S(   2,  20), S(  -0,  17), S(  41,  11), S( 108,  23), S( -62,  82), S(-117,  85),
    S(-137,  53), S(-129,  72), S( -37,  28), S(-122,  74), S(  -5,  31), S( 126,   6), S(-115,  91), S(-219, 101),
    S(-233,  84), S(-182,  91), S(-198,  99), S(-160,  87), S(-184, 104), S(-188, 109), S(-163,  87), S(-252,  87),
    S( -89,  52), S( -94,  58), S(-128,  77), S(-226, 110), S(-271, 129), S(-198, 103), S( -90,  55), S( -88,  30),
    S( -37,  15), S(-156,  71), S( -51,  47), S(-138,  73), S(-140,  71), S(-110,  53), S(   4,  16), S(  45, -30),
    S( -18, -80), S(  -4, -21), S( -23,  -2), S(-131,  11), S(  12, -91), S(-107,  -3), S(  58, -47), S(  53, -84),
    };

    tables[WQueen] = {
    S(333,541), S(392,554), S(411,555), S(385,599), S(419,597), S(415,597), S(503,508), S(492,502),
    S(348,537), S(342,564), S(392,557), S(365,604), S(345,655), S(511,546), S(400,636), S(506,492),
    S(376,467), S(372,511), S(376,570), S(414,549), S(434,616), S(506,598), S(487,598), S(406,589),
    S(371,485), S(375,514), S(388,543), S(374,599), S(383,620), S(386,642), S(381,630), S(389,531),
    S(384,448), S(379,498), S(383,525), S(381,580), S(370,579), S(381,537), S(391,516), S(367,542),
    S(383,412), S(382,452), S(383,492), S(377,484), S(374,496), S(387,504), S(401,487), S(383,442),
    S(364,436), S(376,436), S(395,419), S(392,437), S(399,424), S(420,347), S(394,371), S(341,364),
    S(374,432), S(368,418), S(374,404), S(389,401), S(391,357), S(339,377), S(263,378), S(378,362),
    };

    tables[WBishop] = {
    S(110,119), S(  5,153), S(-27,180), S( 50,162), S( -8,167), S(-80,197), S(101,127), S( 96,135),
    S( 95,116), S(111,140), S(119,143), S( 90,153), S(104,156), S(129,137), S( 75,168), S( 65,131),
    S(102,117), S(108,160), S(141,153), S(143,142), S(177,133), S(289,130), S(144,153), S(155,130),
    S(103,131), S(103,167), S(129,162), S(169,161), S(133,175), S(155,138), S(100,155), S( 88,144),
    S( 97,114), S(115,150), S(102,164), S(139,172), S(152,152), S(107,156), S(101,131), S(112, 75),
    S( 93,112), S(109,145), S(103,168), S(114,154), S(105,181), S(112,140), S(119, 81), S(129,105),
    S(110, 77), S( 99, 98), S(113,103), S( 89,126), S(100,140), S( 75,125), S(133,109), S(118, 40),
    S( 45, 77), S( 64, 88), S( 72, 76), S( 49,127), S( 42,110), S( 66, 97), S(-19,149), S( 91, 70),
    };

    tables[WKnight] = {
    S(-209,  90), S( -59,  88), S( -81, 139), S(  24,  78), S( 179,  61), S( -56, 118), S(  68,  24), S(-215,  51),
    S( -12,  73), S(   5,  89), S(  59, 102), S( 114,  96), S( 105,  71), S( 194,  28), S( -26,  81), S(  10,  59),
    S(   1,  73), S(  52, 103), S(  80, 116), S( 125, 115), S( 197,  73), S( 270,  49), S( 120,  70), S(  31,  65),
    S(  26,  81), S(  32, 115), S(  74, 125), S(  91, 121), S(  58, 131), S( 135,  92), S(  41, 113), S(  76,  67),
    S(  11,  68), S(  23,  88), S(  45, 131), S(  25, 132), S(  40, 128), S(  43, 118), S(  38, 107), S(  11,  56),
    S( -16,  41), S(   6,  73), S(  16,  87), S(  38, 111), S(  32, 113), S(  23,  90), S(  36,  66), S( -19,  47),
    S( -44,  50), S( -42,  58), S( -24,  83), S(  12,  71), S(  13,  65), S(   1,  94), S(  14,  66), S(   5,   9),
    S( -66, -12), S(  -8, -39), S( -49,  40), S( -32,  42), S( -27,  49), S( -15,  33), S( -11,   2), S( -34, -61),
    };

    tables[WRook] = {
    S(169,268), S(186,273), S(210,269), S(227,263), S(263,255), S(184,276), S(218,255), S(250,235),
    S(161,282), S(166,284), S(208,273), S(233,265), S(228,259), S(354,198), S(367,203), S(316,222),
    S(137,269), S(180,255), S(201,254), S(207,244), S(285,214), S(279,221), S(309,210), S(267,209),
    S(114,265), S(124,276), S(167,258), S(177,259), S(177,244), S(218,228), S(143,257), S(118,243),
    S(104,243), S(121,250), S(120,254), S(148,251), S(143,222), S(126,240), S(147,213), S(127,202),
    S( 76,223), S( 94,226), S(109,235), S(115,224), S(125,220), S(123,223), S(171,204), S(116,186),
    S( 76,207), S( 95,216), S(120,221), S( 94,232), S(102,216), S(129,218), S(115,190), S( 44,210),
    S(112,221), S(119,220), S(128,240), S(128,243), S(134,228), S(123,221), S( 85,229), S(111,154),
    };

    tables[WPawn] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 117, 187), S(  74, 205), S( 126, 155), S( 112,  85), S(  68, 128), S( -12, 136), S( -68, 164), S(-206, 205),
    S(   7, 136), S(  26, 129), S(  29, 107), S(  51,  54), S(  85,  36), S( 130,  42), S(  98,  74), S(  25,  84),
    S(  16,  90), S(  37,  79), S(  21,  56), S(  43,  26), S(  52,  33), S(  45,  48), S(  57,  56), S(  37,  59),
    S(   3,  73), S(  33,  69), S(  17,  42), S(  26,  46), S(  29,  37), S(  39,  50), S(  64,  49), S(  39,  36),
    S(   1,  73), S(  28,  67), S(   1,  51), S(  11,  54), S(  32,  67), S(  18,  62), S( 104,  41), S(  59,  31),
    S(  -9,  79), S(  16,  70), S( -12,  61), S( -12,  18), S(  11,  66), S(  67,  68), S( 131,  38), S(  46,  25),
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    };

    for (int piece = 0; piece < NUM_PIECES; ++piece) {
        for (auto& sqr: tables[piece]) {
            sqr += pieceVals[piece];
        }
    }

    return tables;
}();

} // namespace eval