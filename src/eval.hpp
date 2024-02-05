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
constexpr std::array<S, 14> bishopMobility = {
    S( 67, 35), S( 75, 87), S( 82,103), S( 88,125), S( 95,141), S(101,158), S(103,159), S(108,158),
    S(110,160), S(128,143), S(127,146), S(194, 82), S(179,115), S(392,-54), };

constexpr std::array<S, 15> rookMobility = {
    S(111,196), S(120,211), S(128,212), S(135,219), S(134,230), S(144,232), S(149,241), S(158,239),
    S(165,242), S(170,247), S(184,246), S(189,252), S(185,253), S(208,241), S(181,255), };

// other tables
constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 66,191), S( 62,124), S( 25, 72), S(  3, 44), S(-10, 19), S(  9,  6), S(  0,  0),};

constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(901,816), S(293,188), S(387,275), S(348,336), S( 68, 84), };

// misc terms
constexpr auto doubledPawns = S(-16,-34);
constexpr auto chainedPawns = S( 24, 21);
constexpr auto phalanxPawns = S( 11, 10);

// piece square tables
constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S( -29,-185), S( 210,-216), S( -18,  38), S(  91, -86), S( -10,  78), S( 818,-543), S(  55, -23), S(  44, -75),
    S( 811,-525), S(  68, -10), S(  -8,   7), S( -39,  80), S(  46, -17), S(  14,  33), S( 296,-166), S( 309,-105),
    S( 100, -39), S(  60,  -5), S(  13,  14), S(  38,   7), S(  21,  25), S( 112,  25), S( -43,  77), S( -72,  71),
    S(-125,  50), S( -99,  62), S( -53,  38), S(-109,  73), S(   2,  30), S( 124,   9), S(-103,  90), S(-211,  98),
    S(-223,  81), S(-175,  90), S(-193,  97), S(-160,  87), S(-191, 108), S(-193, 110), S(-157,  86), S(-247,  84),
    S( -85,  50), S( -88,  54), S(-131,  77), S(-225, 109), S(-275, 130), S(-203, 103), S( -96,  56), S( -94,  30),
    S( -44,  16), S(-160,  73), S( -69,  48), S(-153,  77), S(-148,  72), S(-111,  52), S(  -4,  17), S(  36, -28),
    S( -28, -81), S(  -6, -22), S( -24,  -0), S(-128,  12), S(   6, -78), S( -96,  -5), S(  57, -49), S(  46, -86),
    };

    tables[QUEEN] = {
    S(325,565), S(377,585), S(397,583), S(369,630), S(399,630), S(395,628), S(491,537), S(488,521),
    S(347,558), S(346,579), S(396,574), S(366,624), S(347,675), S(500,571), S(407,652), S(505,516),
    S(381,481), S(375,531), S(388,579), S(415,570), S(438,636), S(503,622), S(488,621), S(412,607),
    S(372,503), S(384,528), S(397,558), S(382,617), S(389,638), S(387,663), S(393,647), S(394,549),
    S(391,458), S(385,513), S(396,537), S(389,592), S(381,596), S(392,555), S(398,535), S(374,557),
    S(391,422), S(392,464), S(395,504), S(389,499), S(389,509), S(398,520), S(409,503), S(394,455),
    S(380,439), S(387,449), S(405,438), S(405,453), S(411,443), S(438,357), S(408,376), S(351,372),
    S(391,437), S(394,424), S(400,426), S(407,441), S(421,365), S(364,384), S(299,371), S(397,363),
    };

    tables[BISHOP] = {
    S( 86, 65), S(-32, 99), S(-65,126), S( 14,102), S(-38,105), S(-93,129), S( 52, 77), S( 56, 94),
    S( 55, 76), S( 61, 82), S( 70, 84), S( 47, 92), S( 53, 99), S( 77, 82), S( 20,117), S( 38, 82),
    S( 68, 61), S( 67,102), S( 84, 99), S( 88, 85), S(124, 81), S(224, 84), S( 94,101), S(117, 76),
    S( 64, 75), S( 69,100), S( 78,105), S(121,111), S( 83,125), S(107, 80), S( 65, 91), S( 57, 85),
    S( 70, 53), S( 73, 88), S( 68,103), S(102,118), S(110, 99), S( 67, 96), S( 64, 70), S( 94, 24),
    S( 62, 65), S( 80, 91), S( 70,107), S( 82, 91), S( 77,122), S( 84, 82), S( 90, 40), S( 98, 62),
    S( 85, 42), S( 76, 46), S( 85, 50), S( 63, 75), S( 79, 78), S( 56, 66), S(110, 58), S( 93, 24),
    S( 41, 45), S( 57, 45), S( 61, 64), S( 41, 76), S( 43, 63), S( 58, 60), S(-15, 95), S( 74, 44),
    };

    tables[KNIGHT] = {
    S(-193, 104), S( -45, 105), S( -75, 154), S(  14,  98), S( 170,  82), S( -55, 136), S(  72,  42), S(-216,  67),
    S(  -6,  90), S(  11, 105), S(  64, 118), S( 128, 111), S( 114,  88), S( 199,  43), S(   4,  91), S(  26,  75),
    S(   8,  89), S(  66, 116), S(  99, 129), S( 136, 128), S( 209,  87), S( 287,  62), S( 126,  90), S(  45,  79),
    S(  37,  94), S(  46, 126), S(  86, 141), S( 108, 134), S(  76, 142), S( 147, 107), S(  57, 125), S(  87,  80),
    S(  22,  80), S(  38, 102), S(  63, 144), S(  43, 146), S(  59, 141), S(  61, 131), S(  57, 120), S(  24,  70),
    S(  -2,  53), S(  20,  88), S(  33, 102), S(  55, 126), S(  54, 123), S(  44, 104), S(  50,  80), S(  -3,  59),
    S( -22,  59), S( -27,  74), S(  -7,  99), S(  33,  87), S(  33,  81), S(  21, 107), S(  30,  78), S(  20,  24),
    S( -53,   0), S(  16,  -7), S( -15,  55), S(   2,  54), S(   6,  65), S(  18,  44), S(  14,  22), S( -21, -52),
    };

    tables[ROOK] = {
    S( 90,174), S(104,179), S(118,176), S(133,170), S(170,165), S(109,184), S(159,158), S(192,138),
    S( 82,190), S( 90,191), S(129,178), S(158,170), S(154,167), S(274,108), S(308,109), S(266,127),
    S( 71,179), S(120,161), S(140,158), S(148,148), S(225,118), S(231,125), S(267,113), S(219,116),
    S( 58,174), S( 72,183), S(111,164), S(120,162), S(122,151), S(168,136), S(110,165), S( 75,151),
    S( 50,154), S( 70,157), S( 70,160), S( 93,157), S( 94,129), S( 85,148), S(111,121), S( 84,112),
    S( 34,131), S( 45,135), S( 61,143), S( 69,131), S( 82,124), S( 93,127), S(134,111), S( 83, 92),
    S( 36,111), S( 51,118), S( 74,125), S( 56,135), S( 67,120), S( 93,116), S( 79, 89), S( 13,113),
    S( 74,121), S( 81,118), S( 92,134), S( 97,132), S(107,119), S(100,119), S( 54,127), S( 85, 60),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 117, 191), S(  68, 210), S( 115, 164), S(  97,  92), S(  55, 134), S( -15, 141), S( -70, 170), S(-185, 202),
    S(  12, 132), S(  23, 127), S(  26, 104), S(  50,  47), S(  82,  32), S( 131,  39), S(  94,  70), S(  30,  77),
    S(  19,  85), S(  28,  72), S(  17,  53), S(  37,  20), S(  49,  30), S(  42,  45), S(  54,  49), S(  42,  53),
    S(   4,  64), S(  19,  60), S(  16,  31), S(  16,  36), S(  25,  33), S(  35,  40), S(  53,  40), S(  28,  29),
    S(  -2,  63), S(   5,  56), S(  -8,  49), S(  -2,  49), S(  17,  58), S(  15,  58), S(  84,  23), S(  46,  19),
    S(  -2,  66), S(   6,  64), S( -13,  58), S(  -1,  23), S(  11,  69), S(  72,  56), S( 116,  27), S(  44,  16),
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