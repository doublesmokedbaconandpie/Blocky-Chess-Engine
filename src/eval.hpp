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
    S(  0,  0), S( 73,184), S( 62,111), S( 22, 60), S( -4, 31), S(-21,  7), S( -0, -4), S(  0,  0),};

constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(921,808), S(352,277), S(403,284), S(440,488), S( 74, 91), };

// misc terms
constexpr S doubledPawns{S(-27,-49)};

// piece square tables
constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[WKing] = {
    S( -30,-177), S( 199,-210), S( -30,  42), S(  87, -82), S(  -1,  71), S( 821,-547), S(  38, -17), S(  41, -74),
    S( 794,-516), S(  52,  -4), S( -13,   9), S( -38,  76), S(  21, -15), S(  -5,  36), S( 278,-163), S( 307,-105),
    S(  76, -29), S(  31,   5), S(  -1,  19), S(  21,   8), S(  33,  15), S( 107,  23), S( -68,  83), S(-121,  87),
    S(-136,  52), S(-112,  66), S( -52,  34), S(-114,  72), S(   2,  26), S( 118,   9), S(-117,  92), S(-221, 102),
    S(-238,  86), S(-183,  92), S(-198,  98), S(-156,  85), S(-187, 105), S(-191, 108), S(-166,  88), S(-248,  86),
    S( -89,  52), S( -89,  56), S(-128,  77), S(-224, 109), S(-270, 129), S(-198, 103), S( -90,  55), S( -88,  29),
    S( -37,  16), S(-158,  72), S( -50,  46), S(-139,  73), S(-140,  71), S(-109,  52), S(   5,  15), S(  46, -29),
    S( -17, -81), S(  -4, -22), S( -23,  -3), S(-130,  10), S(  13, -91), S(-106,  -3), S(  59, -47), S(  54, -84),
    };

    tables[WQueen] = {
    S(331,544), S(391,558), S(413,555), S(386,599), S(420,600), S(421,593), S(507,507), S(494,501),
    S(349,538), S(342,564), S(393,556), S(365,606), S(345,657), S(508,550), S(403,634), S(508,492),
    S(375,471), S(372,514), S(379,567), S(414,550), S(436,615), S(508,599), S(488,600), S(406,593),
    S(370,487), S(375,517), S(391,542), S(373,600), S(384,621), S(387,641), S(383,631), S(389,533),
    S(385,449), S(379,499), S(385,524), S(381,579), S(370,581), S(382,538), S(391,517), S(367,543),
    S(384,413), S(383,452), S(383,494), S(378,483), S(375,497), S(387,505), S(402,487), S(384,442),
    S(365,434), S(376,438), S(396,419), S(391,440), S(399,425), S(421,347), S(393,373), S(340,365),
    S(376,429), S(369,419), S(374,407), S(389,401), S(392,358), S(340,380), S(270,373), S(379,358),
    };

    tables[WBishop] = {
    S(115,116), S(  9,152), S(-30,181), S( 53,162), S(  5,163), S(-64,192), S(101,126), S( 95,136),
    S( 94,120), S(109,140), S(120,143), S( 88,154), S(104,157), S(126,140), S( 71,171), S( 69,131),
    S(104,117), S(109,161), S(140,153), S(143,143), S(178,134), S(289,130), S(145,155), S(159,130),
    S(103,132), S(105,166), S(129,163), S(170,160), S(134,175), S(156,137), S(100,156), S( 92,142),
    S( 98,115), S(115,150), S(104,164), S(140,172), S(153,151), S(108,156), S(101,132), S(114, 76),
    S( 93,112), S(111,145), S(104,169), S(115,154), S(106,181), S(114,140), S(121, 81), S(131,105),
    S(112, 77), S( 99, 98), S(115,103), S( 89,127), S(101,140), S( 76,124), S(134,108), S(120, 41),
    S( 48, 76), S( 65, 91), S( 72, 76), S( 51,127), S( 43,110), S( 67, 97), S( -9,145), S( 92, 67),
    };

    tables[WKnight] = {
    S(-211,  91), S( -49,  87), S( -81, 140), S(  26,  76), S( 179,  62), S( -47, 116), S(  65,  25), S(-210,  49),
    S( -14,  73), S(   4,  90), S(  57, 103), S( 116,  96), S( 104,  71), S( 199,  26), S( -24,  81), S(  21,  56),
    S(   1,  73), S(  53, 102), S(  82, 115), S( 124, 116), S( 196,  73), S( 276,  48), S( 120,  72), S(  33,  66),
    S(  24,  82), S(  31, 114), S(  74, 125), S(  90, 121), S(  59, 130), S( 135,  92), S(  41, 113), S(  74,  69),
    S(   9,  68), S(  23,  89), S(  45, 132), S(  24, 131), S(  39, 128), S(  42, 118), S(  39, 107), S(  10,  56),
    S( -17,  39), S(   5,  73), S(  15,  86), S(  38, 111), S(  32, 112), S(  23,  90), S(  35,  66), S( -20,  46),
    S( -44,  49), S( -45,  63), S( -25,  84), S(  11,  72), S(  12,  65), S(   0,  94), S(  12,  66), S(   5,  11),
    S( -68, -15), S( -11, -39), S( -50,  40), S( -34,  40), S( -27,  49), S( -15,  34), S( -13,   2), S( -34, -62),
    };

    tables[WRook] = {
    S(170,268), S(187,272), S(211,269), S(226,263), S(263,254), S(186,275), S(221,254), S(250,235),
    S(161,282), S(167,283), S(208,273), S(234,265), S(227,259), S(354,197), S(363,204), S(320,222),
    S(137,270), S(181,255), S(201,254), S(207,244), S(286,213), S(282,219), S(313,208), S(268,210),
    S(117,264), S(125,277), S(170,258), S(178,258), S(177,245), S(221,228), S(148,256), S(119,244),
    S(105,244), S(121,250), S(121,255), S(148,251), S(143,222), S(127,240), S(149,213), S(127,202),
    S( 77,223), S( 94,226), S(109,235), S(115,224), S(124,220), S(125,222), S(173,203), S(117,185),
    S( 76,207), S( 96,215), S(120,221), S( 93,231), S(102,215), S(129,217), S(114,190), S( 41,212),
    S(111,219), S(119,220), S(128,239), S(128,241), S(134,227), S(122,220), S( 86,230), S(110,152),
    };

    tables[WPawn] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 117, 186), S(  75, 203), S( 125, 155), S( 113,  84), S(  70, 125), S(  -3, 131), S( -75, 165), S(-204, 204),
    S(   7, 136), S(  25, 129), S(  29, 108), S(  51,  54), S(  87,  35), S( 132,  42), S(  97,  76), S(  25,  83),
    S(  17,  91), S(  37,  81), S(  24,  59), S(  46,  28), S(  55,  34), S(  47,  49), S(  57,  57), S(  34,  59),
    S(   4,  74), S(  33,  72), S(  21,  44), S(  28,  48), S(  31,  40), S(  41,  52), S(  65,  52), S(  35,  37),
    S(   2,  74), S(  28,  70), S(   7,  56), S(  12,  55), S(  35,  69), S(  30,  70), S( 107,  44), S(  54,  31),
    S(  -9,  79), S(  15,  72), S(  -9,  64), S( -11,  19), S(  12,  68), S(  69,  69), S( 132,  39), S(  41,  25),
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