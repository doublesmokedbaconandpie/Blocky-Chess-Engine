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
S evalMobilityScores(const PieceSets& pieceSets, bool isWhite);
int evalMopUpScore(const PieceSets& pieceSets, int eval);

int getPiecePhase(pieceTypes piece);
S getPSQTVal(Square square, pieceTypes currPiece);

// bitmasking tricks
bool isPassedPawn(Square pawn, uint64_t enemyPawns, bool isWhite);
uint64_t getDoubledPawnsMask(uint64_t allyPawnSet, bool isWhite);
uint64_t getChainedPawnsMask(uint64_t allyPawnSet, bool isWhite);
uint64_t getPhalanxPawnsMask(uint64_t allyPawnSet, bool isWhite);
uint64_t getMobilitySquares(const PieceSets& pieceSets, bool isWhite);
int getPieceMobility(pieceTypes piece, Square sq, uint64_t mobilitySquares, uint64_t allPieces);

/*************
 * Evaluation Terms
**************/

// mobility tables
constexpr std::array<S, 14> bishopMobility = {};

// other tables
constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 78,187), S( 65,121), S( 26, 70), S(  4, 44), S(-10, 20), S(  9,  8), S(  0,  0),};

constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(922,810), S(352,277), S(401,285), S(441,489), S( 67, 84), };

// misc terms
constexpr auto doubledPawns = S(-13,-33);
constexpr auto chainedPawns = S( 25, 22);
constexpr auto phalanxPawns = S(  9,  9);

// piece square tables
constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S( -35,-183), S( 203,-213), S( -27,  39), S(  87, -85), S( -10,  74), S( 819,-548), S(  52, -23), S(  45, -76),
    S( 795,-519), S(  55,  -8), S( -26,  12), S( -50,  78), S(  29, -17), S(  -3,  35), S( 295,-169), S( 313,-106),
    S(  78, -30), S(  42,  -0), S(  -4,  18), S(  35,   1), S(  40,  12), S( 101,  27), S( -65,  84), S(-114,  86),
    S(-134,  51), S(-114,  66), S( -54,  34), S(-105,  68), S(   8,  25), S( 124,   7), S(-110,  92), S(-222, 102),
    S(-239,  85), S(-185,  92), S(-197,  97), S(-157,  84), S(-187, 106), S(-187, 108), S(-160,  87), S(-249,  86),
    S( -90,  50), S( -85,  54), S(-129,  76), S(-222, 107), S(-274, 130), S(-195, 102), S( -87,  55), S( -90,  30),
    S( -35,  13), S(-155,  71), S( -52,  45), S(-141,  73), S(-140,  70), S(-108,  52), S(   8,  15), S(  50, -31),
    S( -19, -80), S(  -4, -23), S( -24,  -4), S(-134,  11), S(  10, -91), S(-107,  -3), S(  59, -48), S(  53, -85),
    };

    tables[QUEEN] = {
    S(334,547), S(395,559), S(416,558), S(390,603), S(425,600), S(426,595), S(506,513), S(493,507),
    S(351,543), S(348,564), S(396,559), S(367,609), S(347,660), S(511,551), S(413,635), S(508,498),
    S(380,470), S(376,516), S(384,568), S(418,552), S(445,616), S(510,602), S(490,604), S(409,596),
    S(374,487), S(381,517), S(396,545), S(379,602), S(388,625), S(389,647), S(388,635), S(393,535),
    S(389,452), S(383,502), S(391,525), S(386,581), S(374,583), S(387,541), S(395,519), S(372,543),
    S(388,411), S(389,457), S(387,497), S(383,484), S(380,499), S(390,508), S(406,489), S(390,443),
    S(371,432), S(380,437), S(401,422), S(396,444), S(404,427), S(425,348), S(398,371), S(345,368),
    S(379,431), S(372,422), S(378,411), S(392,405), S(396,358), S(343,381), S(273,372), S(383,361),
    };

    tables[BISHOP] = {
    S(116,116), S( 12,153), S(-31,183), S( 57,162), S(  3,166), S(-62,193), S(100,128), S( 93,141),
    S( 93,122), S(113,139), S(120,144), S( 90,155), S(105,158), S(129,141), S( 72,172), S( 67,134),
    S(106,119), S(110,163), S(141,155), S(145,144), S(181,136), S(287,134), S(142,159), S(161,131),
    S(103,133), S(106,168), S(128,166), S(173,162), S(137,176), S(157,139), S(102,158), S( 94,141),
    S(100,115), S(113,153), S(107,165), S(141,175), S(155,153), S(109,158), S(103,134), S(115, 78),
    S( 92,114), S(112,148), S(104,170), S(117,155), S(106,184), S(116,142), S(123, 85), S(129,105),
    S(109, 82), S(102, 99), S(116,105), S( 91,129), S(104,141), S( 78,125), S(137,112), S(120, 48),
    S( 52, 76), S( 66, 92), S( 73, 79), S( 56,127), S( 49,109), S( 69, 99), S( -6,147), S( 93, 71),
    };

    tables[KNIGHT] = {
    S(-210,  89), S( -58,  93), S( -72, 137), S(  21,  78), S( 174,  63), S( -50, 119), S(  65,  27), S(-216,  53),
    S( -14,  75), S(   3,  91), S(  58, 104), S( 120,  96), S( 108,  71), S( 203,  23), S( -16,  79), S(  15,  61),
    S(   2,  74), S(  59, 102), S(  87, 116), S( 127, 116), S( 203,  72), S( 279,  47), S( 122,  74), S(  37,  65),
    S(  27,  82), S(  33, 115), S(  75, 127), S(  96, 121), S(  64, 131), S( 138,  93), S(  44, 115), S(  75,  69),
    S(  11,  69), S(  24,  89), S(  48, 132), S(  27, 133), S(  42, 128), S(  44, 121), S(  43, 107), S(  12,  58),
    S( -15,  42), S(   9,  74), S(  18,  90), S(  39, 113), S(  35, 113), S(  25,  94), S(  37,  68), S( -17,  47),
    S( -43,  51), S( -44,  63), S( -22,  86), S(  13,  73), S(  14,  66), S(  -1,  96), S(  15,  66), S(   7,  11),
    S( -69, -14), S( -10, -37), S( -47,  42), S( -33,  41), S( -24,  51), S( -10,  36), S( -13,   2), S( -33, -65),
    };

    tables[ROOK] = {
    S(172,269), S(188,274), S(212,270), S(228,264), S(265,255), S(188,277), S(223,255), S(253,235),
    S(160,284), S(167,284), S(207,274), S(234,266), S(227,261), S(349,200), S(365,205), S(318,224),
    S(136,272), S(181,256), S(203,255), S(210,244), S(289,215), S(283,221), S(313,210), S(270,212),
    S(119,265), S(128,278), S(172,259), S(181,260), S(179,246), S(222,231), S(151,259), S(122,246),
    S(106,246), S(122,253), S(123,256), S(151,252), S(146,223), S(132,242), S(153,214), S(131,205),
    S( 82,222), S( 97,229), S(111,238), S(115,227), S(125,222), S(128,224), S(174,206), S(125,185),
    S( 78,207), S( 98,215), S(123,221), S( 95,232), S(104,216), S(130,218), S(114,190), S( 45,211),
    S(113,219), S(121,220), S(130,239), S(131,241), S(136,228), S(123,221), S( 86,231), S(111,152),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 121, 190), S(  76, 206), S( 128, 157), S( 112,  87), S(  72, 126), S(   7, 131), S( -69, 166), S(-198, 208),
    S(  12, 130), S(  26, 124), S(  32, 103), S(  54,  47), S(  92,  29), S( 138,  38), S(  96,  69), S(  34,  77),
    S(  18,  83), S(  31,  71), S(  20,  52), S(  41,  20), S(  54,  31), S(  47,  43), S(  59,  46), S(  42,  52),
    S(   1,  64), S(  20,  59), S(  14,  32), S(  18,  35), S(  24,  32), S(  30,  41), S(  55,  37), S(  27,  29),
    S(  -9,  64), S(   5,  56), S(  -8,  49), S(  -6,  50), S(  11,  59), S(   7,  59), S(  84,  23), S(  43,  20),
    S( -13,  70), S(   5,  63), S( -13,  57), S( -12,  12), S(   5,  65), S(  62,  58), S( 116,  26), S(  42,  19),
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