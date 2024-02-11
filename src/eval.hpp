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
#include "utils/types.hpp"

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
        int getRawEval(const PieceSets& pieceSets, bool isWhiteTurn);
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
uint64_t getPhalanxPawnsMask(uint64_t allyPawnSet);
uint64_t getMobilitySquares(const PieceSets& pieceSets, bool isWhite);
int getPieceMobility(pieceTypes piece, Square sq, uint64_t mobilitySquares, uint64_t allPieces);

/*************
 * Evaluation Terms
**************/

// mobility tables
constexpr std::array<S, 9> knightMobility = {
    S( 28,-55), S( 40, 56), S( 50,105), S( 56,125), S( 60,139), S( 62,152), S( 68,145), S( 79,124),
    S(137, 40), };

constexpr std::array<S, 14> bishopMobility = {
    S( 65, 39), S( 74, 90), S( 82,106), S( 88,127), S( 96,144), S(101,160), S(104,161), S(110,159),
    S(112,161), S(130,143), S(129,146), S(197, 82), S(184,113), S(395,-55), };

constexpr std::array<S, 15> rookMobility = {
    S(112,195), S(123,211), S(131,212), S(138,219), S(137,232), S(147,233), S(152,244), S(161,241),
    S(169,244), S(174,249), S(190,248), S(197,253), S(192,254), S(215,242), S(190,254), };

// other tables
constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 65,192), S( 62,124), S( 25, 72), S(  3, 44), S(-10, 19), S(  8,  6), S(  0,  0),};

constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(899,818), S(289,186), S(347,193), S(343,333), S( 68, 84), };

// misc terms
constexpr auto doubledPawns = S(-16,-33);
constexpr auto chainedPawns = S( 23, 21);
constexpr auto phalanxPawns = S( 10, 10);
constexpr auto tempo = S();

// piece square tables
constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S( -23,-182), S( 208,-213), S( -40,  44), S(  92, -85), S( -17,  82), S( 801,-530), S(  58, -27), S(  44, -77),
    S( 797,-520), S(  75, -13), S( -21,  13), S( -39,  80), S(  53, -22), S(  16,  31), S( 284,-161), S( 299,-101),
    S(  91, -35), S(  66,  -8), S(  10,  17), S(  33,  10), S(  27,  24), S( 119,  23), S( -36,  73), S( -70,  68),
    S(-129,  51), S(-107,  64), S( -54,  40), S(-108,  75), S(   2,  31), S( 131,   8), S(-102,  89), S(-198,  93),
    S(-219,  78), S(-182,  92), S(-199, 100), S(-163,  90), S(-197, 111), S(-191, 110), S(-151,  84), S(-245,  83),
    S( -88,  50), S( -89,  54), S(-128,  76), S(-227, 111), S(-276, 130), S(-199, 102), S( -96,  56), S( -94,  31),
    S( -45,  16), S(-161,  72), S( -72,  49), S(-153,  77), S(-147,  72), S(-112,  51), S(  -4,  16), S(  36, -28),
    S( -30, -79), S(  -5, -23), S( -24,  -1), S(-129,  12), S(   6, -79), S( -96,  -5), S(  57, -50), S(  45, -86),
    };

    tables[QUEEN] = {
    S(326,564), S(374,589), S(392,588), S(363,636), S(398,633), S(394,629), S(487,541), S(489,520),
    S(347,558), S(344,582), S(394,576), S(368,621), S(343,680), S(499,572), S(399,660), S(505,516),
    S(380,483), S(373,533), S(385,583), S(413,573), S(437,636), S(500,627), S(483,626), S(408,612),
    S(369,506), S(384,527), S(397,559), S(381,619), S(388,640), S(386,665), S(394,647), S(391,555),
    S(391,458), S(384,514), S(396,537), S(389,593), S(381,597), S(391,556), S(396,537), S(374,558),
    S(390,422), S(391,466), S(396,505), S(388,504), S(389,509), S(398,520), S(408,504), S(391,460),
    S(380,441), S(386,452), S(403,443), S(407,452), S(412,443), S(439,360), S(407,380), S(349,376),
    S(390,440), S(395,426), S(400,427), S(407,442), S(423,363), S(366,386), S(304,363), S(399,362),
    };

    tables[BISHOP] = {
    S( 92, 64), S(-32,101), S(-65,127), S(  5,105), S(-40,107), S(-94,130), S( 53, 79), S( 58, 91),
    S( 54, 79), S( 61, 85), S( 72, 85), S( 46, 94), S( 53,101), S( 80, 82), S( 28,115), S( 38, 83),
    S( 69, 64), S( 66,104), S( 86,101), S( 88, 88), S(124, 83), S(224, 86), S( 95,102), S(118, 79),
    S( 64, 77), S( 70,101), S( 79,108), S(121,113), S( 85,127), S(107, 82), S( 67, 92), S( 58, 88),
    S( 73, 54), S( 74, 90), S( 70,105), S(103,120), S(111,101), S( 67, 99), S( 66, 72), S( 98, 24),
    S( 63, 66), S( 81, 92), S( 71,110), S( 83, 94), S( 77,124), S( 85, 85), S( 91, 44), S( 99, 64),
    S( 87, 41), S( 76, 48), S( 85, 54), S( 66, 76), S( 81, 80), S( 59, 68), S(111, 61), S( 96, 23),
    S( 42, 47), S( 61, 46), S( 62, 68), S( 43, 77), S( 48, 63), S( 59, 63), S( -9, 95), S( 73, 48),
    };

    tables[KNIGHT] = {
    S(-189,  97), S( -48,  63), S( -89, 101), S(   1,  46), S( 145,  34), S( -55,  80), S(  60,   7), S(-225,  65),
    S( -19,  58), S(  -8,  57), S(  42,  57), S( 109,  47), S(  97,  27), S( 175, -19), S(  -7,  43), S(  16,  42),
    S( -13,  44), S(  40,  55), S(  51, 103), S(  96,  91), S( 162,  56), S( 243,  28), S(  99,  28), S(  28,  36),
    S(  19,  52), S(  22,  65), S(  54,  96), S(  78,  86), S(  47,  92), S( 114,  62), S(  35,  64), S(  72,  42),
    S(  10,  46), S(  17,  41), S(  34, 100), S(  21,  92), S(  36,  88), S(  43,  81), S(  42,  65), S(  17,  54),
    S( -19,  17), S(   1,  30), S(   8,  50), S(  30,  76), S(  30,  73), S(  20,  50), S(  31,  25), S( -10,  50),
    S( -31,  35), S( -36,  35), S( -25,  41), S(  19,  33), S(  22,  30), S(   1,  50), S(  17,  49), S(  10,  12),
    S( -56,  -8), S(   4, -21), S( -27,  14), S(  -3,  23), S(  -1,  31), S(   6,   7), S(   3,   6), S( -25,  -7),
    };

    tables[ROOK] = {
    S( 89,177), S(103,182), S(117,180), S(134,171), S(170,167), S(111,185), S(163,158), S(197,138),
    S( 82,193), S( 89,193), S(128,181), S(157,172), S(154,169), S(274,110), S(312,109), S(267,128),
    S( 69,181), S(120,163), S(138,162), S(149,150), S(227,119), S(233,126), S(269,114), S(220,118),
    S( 59,176), S( 73,185), S(111,167), S(123,163), S(124,153), S(168,139), S(111,167), S( 76,154),
    S( 51,156), S( 71,159), S( 72,162), S( 94,159), S( 96,130), S( 87,150), S(113,124), S( 87,114),
    S( 35,134), S( 47,137), S( 64,145), S( 70,134), S( 83,126), S( 96,128), S(136,114), S( 87, 94),
    S( 37,114), S( 52,120), S( 75,128), S( 59,137), S( 70,122), S( 95,119), S( 83, 90), S( 15,115),
    S( 75,124), S( 84,119), S( 94,136), S( 99,134), S(110,120), S(103,120), S( 58,128), S( 86, 63),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 118, 189), S(  69, 210), S( 113, 166), S(  95,  93), S(  48, 137), S( -14, 140), S( -73, 171), S(-184, 201),
    S(  12, 131), S(  23, 127), S(  25, 105), S(  49,  48), S(  79,  33), S( 128,  40), S(  94,  70), S(  29,  77),
    S(  19,  84), S(  27,  71), S(  15,  54), S(  36,  19), S(  48,  28), S(  41,  45), S(  53,  47), S(  40,  54),
    S(   6,  63), S(  17,  61), S(  16,  31), S(  15,  36), S(  25,  33), S(  34,  41), S(  51,  40), S(  29,  29),
    S(  -2,  64), S(   6,  56), S(  -8,  51), S(  -4,  51), S(  17,  59), S(  14,  60), S(  84,  23), S(  45,  20),
    S(  -0,  65), S(   7,  63), S( -12,  58), S(   4,  22), S(  12,  68), S(  72,  56), S( 116,  27), S(  46,  15),
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