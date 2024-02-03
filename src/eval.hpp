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
S evalPawns(const PieceSets& pieceSets, bool isWhite);
int mopUpScore(const PieceSets& pieceSets, int eval);
int getPiecePhase(pieceTypes piece);
S getPSQTVal(Square square, pieceTypes currPiece);

// bitmasking tricks
bool isPassedPawn(Square pawn, uint64_t enemyPawns, bool isWhitePawn);
uint64_t getDoubledPawnsMask(uint64_t allyPawnSet, bool isWhitePawn);
uint64_t getChainedPawnsMask(uint64_t allyPawnSet, bool isWhitePawn);

/*************
 * Evaluation Terms
**************/

// tables
constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 75,187), S( 62,117), S( 25, 67), S(  0, 42), S(-14, 17), S(  4,  5), S(  0,  0),};

constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(924,810), S(357,278), S(401,284), S(442,489), S( 71, 85), };

// misc terms
constexpr auto doubledPawns = S(-18,-36);
constexpr auto chainedPawns = S( 19, 19);

// piece square tables
constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[WKing] = {
    S( -30,-181), S( 203,-213), S( -21,  37), S(  89, -85), S(  -7,  73), S( 818,-547), S(  51, -22), S(  43, -75),
    S( 797,-518), S(  51,  -5), S(  -5,   4), S( -46,  78), S(  31, -18), S(  -7,  37), S( 286,-166), S( 310,-105),
    S(  81, -31), S(  32,   5), S(   7,  16), S(  36,   2), S(  36,  14), S( 101,  26), S( -65,  85), S(-115,  87),
    S(-134,  51), S(-109,  65), S( -54,  35), S(-104,  68), S(   9,  24), S( 126,   7), S(-112,  92), S(-216, 101),
    S(-242,  87), S(-183,  92), S(-196,  97), S(-157,  84), S(-186, 106), S(-189, 109), S(-162,  88), S(-249,  86),
    S( -91,  51), S( -89,  55), S(-128,  76), S(-224, 108), S(-274, 130), S(-196, 102), S( -89,  56), S( -90,  30),
    S( -37,  14), S(-157,  72), S( -53,  45), S(-142,  74), S(-142,  71), S(-110,  52), S(   6,  14), S(  50, -31),
    S( -21, -79), S(  -4, -23), S( -23,  -4), S(-135,  11), S(   9, -90), S(-108,  -2), S(  57, -47), S(  51, -85),
    };

    tables[WQueen] = {
    S(331,548), S(391,560), S(414,557), S(388,601), S(422,601), S(424,594), S(507,511), S(491,506),
    S(349,540), S(345,564), S(393,558), S(365,608), S(346,658), S(510,549), S(413,631), S(506,495),
    S(377,471), S(373,515), S(383,565), S(416,551), S(442,616), S(507,603), S(489,602), S(408,595),
    S(371,489), S(378,518), S(394,544), S(376,602), S(386,624), S(387,647), S(385,636), S(391,535),
    S(386,452), S(380,502), S(388,527), S(384,580), S(371,584), S(384,542), S(392,519), S(369,542),
    S(386,412), S(385,456), S(384,497), S(380,486), S(376,500), S(387,508), S(402,489), S(388,443),
    S(368,435), S(378,438), S(398,421), S(392,444), S(401,426), S(422,349), S(396,373), S(342,369),
    S(378,429), S(370,422), S(376,408), S(389,404), S(393,360), S(342,378), S(273,370), S(381,359),
    };

    tables[WBishop] = {
    S(111,116), S(  5,152), S(-42,184), S( 50,161), S( -4,166), S(-71,193), S(103,126), S( 89,140),
    S( 88,121), S(109,138), S(115,144), S( 85,154), S(101,157), S(125,140), S( 68,171), S( 63,133),
    S(101,118), S(106,162), S(136,154), S(140,143), S(177,136), S(284,134), S(140,158), S(157,131),
    S( 98,133), S(101,167), S(124,165), S(168,161), S(130,176), S(152,138), S( 98,158), S( 89,142),
    S( 96,114), S(108,152), S(101,165), S(136,174), S(150,152), S(104,157), S( 98,133), S(111, 77),
    S( 87,113), S(108,146), S( 99,169), S(113,154), S(102,182), S(110,141), S(118, 84), S(124,104),
    S(104, 80), S( 97, 98), S(111,104), S( 85,128), S( 99,140), S( 72,125), S(132,112), S(113, 46),
    S( 46, 76), S( 59, 92), S( 68, 77), S( 50,127), S( 42,109), S( 65, 98), S(-11,145), S( 89, 68),
    };

    tables[WKnight] = {
    S(-208,  91), S( -50,  89), S( -77, 139), S(  22,  78), S( 181,  61), S( -51, 119), S(  63,  28), S(-202,  46),
    S( -12,  74), S(   5,  90), S(  58, 104), S( 120,  96), S( 108,  71), S( 204,  23), S( -17,  79), S(  19,  59),
    S(   3,  73), S(  59, 101), S(  85, 116), S( 127, 116), S( 202,  73), S( 280,  48), S( 125,  73), S(  37,  66),
    S(  27,  82), S(  33, 115), S(  76, 127), S(  96, 122), S(  63, 131), S( 139,  93), S(  44, 115), S(  75,  70),
    S(  11,  69), S(  24,  89), S(  48, 132), S(  27, 133), S(  42, 128), S(  44, 120), S(  43, 108), S(  12,  58),
    S( -15,  41), S(   9,  74), S(  18,  90), S(  40, 113), S(  35, 113), S(  25,  93), S(  37,  68), S( -18,  47),
    S( -44,  50), S( -44,  63), S( -23,  86), S(  13,  73), S(  14,  66), S(  -0,  95), S(  15,  67), S(   7,  10),
    S( -67, -14), S( -10, -38), S( -47,  41), S( -32,  41), S( -25,  50), S( -11,  36), S( -13,   3), S( -33, -63),
    };

    tables[WRook] = {
    S(170,269), S(186,274), S(211,269), S(228,263), S(264,255), S(188,275), S(220,255), S(252,234),
    S(159,283), S(165,284), S(205,274), S(233,266), S(225,260), S(351,198), S(360,205), S(315,223),
    S(136,271), S(180,256), S(201,254), S(208,244), S(288,214), S(281,220), S(311,210), S(269,210),
    S(117,265), S(127,278), S(171,259), S(180,259), S(177,246), S(222,230), S(150,257), S(121,245),
    S(104,245), S(120,252), S(122,255), S(149,252), S(144,223), S(130,241), S(150,214), S(129,204),
    S( 79,223), S( 95,229), S(110,237), S(114,226), S(124,221), S(126,223), S(171,205), S(122,185),
    S( 76,207), S( 97,214), S(121,220), S( 93,231), S(103,215), S(128,218), S(116,189), S( 42,212),
    S(112,219), S(119,220), S(129,239), S(129,241), S(134,227), S(121,221), S( 85,231), S(110,152),
    };

    tables[WPawn] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 118, 190), S(  74, 206), S( 126, 157), S( 112,  86), S(  69, 127), S(   6, 131), S( -71, 166), S(-201, 207),
    S(  11, 133), S(  26, 126), S(  31, 105), S(  52,  50), S(  89,  33), S( 134,  40), S(  97,  72), S(  32,  80),
    S(  16,  85), S(  32,  73), S(  20,  54), S(  41,  22), S(  52,  32), S(  47,  44), S(  57,  49), S(  40,  54),
    S(  -0,  66), S(  23,  61), S(  15,  34), S(  20,  37), S(  24,  34), S(  32,  43), S(  56,  40), S(  27,  30),
    S(  -8,  66), S(   9,  58), S(  -6,  50), S(  -2,  51), S(  15,  61), S(  13,  61), S(  89,  27), S(  46,  22),
    S( -10,  72), S(  10,  66), S( -10,  59), S( -12,  12), S(   9,  68), S(  66,  62), S( 126,  29), S(  44,  21),
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