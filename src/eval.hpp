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

inline constexpr int TOTAL_PHASE = 24;
inline constexpr int PAWN_HASH_SIZE = 1024;

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
S evalPieces(const PieceSets& pieceSets, bool isWhite);
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

inline bool isBishopPair(uint64_t allyBishops) {
    return popcount(allyBishops) > 1;
}

/*************
 * Evaluation Terms
**************/

// mobility tables
inline constexpr std::array<S, 9> knightMobility = {
    S( 26,-61), S( 40, 49), S( 50, 98), S( 57,122), S( 62,135), S( 65,147), S( 71,139), S( 83,118),
    S(140, 35), };

inline constexpr std::array<S, 14> bishopMobility = {
    S( 70, 32), S( 79, 79), S( 87, 98), S( 93,119), S(101,137), S(106,153), S(109,157), S(114,154),
    S(117,158), S(133,141), S(132,145), S(196, 83), S(185,112), S(396,-54), };

inline constexpr std::array<S, 15> rookMobility = {
    S(119,185), S(130,202), S(137,206), S(144,212), S(143,226), S(153,228), S(158,238), S(168,237),
    S(177,239), S(182,243), S(197,243), S(204,249), S(205,247), S(223,236), S(206,244), };

// other tables
inline constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 73,186), S( 60,127), S( 29, 70), S(  1, 44), S( -7, 19), S(  9,  8), S(  0,  0),
    };

inline constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(948,797), S(302,183), S(362,189), S(362,330), S( 71, 81), };

// misc terms
inline constexpr auto doubledPawns = S(-19,-31);
inline constexpr auto chainedPawns = S( 23, 21);
inline constexpr auto phalanxPawns = S( 12, 11);
inline constexpr auto bishopPair = S();
inline constexpr int tempo = 11;

// piece square tables
inline constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S(  45,-186), S( 204,-183), S( -15,  24), S(  96, -68), S(  28,  32), S( 701,-480), S(  89, -24), S( -21, -46),
    S( 707,-486), S( -62,  52), S( -38,  30), S( 125, -37), S(  17,   8), S(  45,  39), S( 188, -96), S( 206, -66),
    S(   8,  -7), S(  12,  26), S( -25,  34), S( -28,  43), S(  28,  23), S(  98,  27), S(  85,  16), S(  51,  27),
    S(-129,  49), S(-131,  74), S( -81,  55), S( -39,  50), S( -16,  40), S( 123,  16), S( -18,  55), S(-127,  64),
    S(-195,  66), S(-208,  95), S(-206, 101), S(-154,  91), S(-160,  98), S(-161,  98), S(-151,  85), S(-229,  73),
    S( -81,  38), S( -91,  50), S( -99,  67), S(-217, 108), S(-259, 123), S(-183,  94), S( -82,  46), S( -91,  23),
    S( -55,  15), S(-143,  56), S( -69,  46), S(-141,  64), S(-132,  62), S(-107,  46), S(   4,  10), S(  38, -33),
    S( -29, -74), S(  -9, -27), S( -21,  -5), S(-127,   7), S(  12, -82), S( -85, -10), S(  60, -54), S(  49, -91),
    };

    tables[QUEEN] = {
    S(334,546), S(379,563), S(403,576), S(378,610), S(411,608), S(427,588), S(518,507), S(497,503),
    S(346,543), S(345,569), S(398,561), S(373,608), S(353,660), S(505,552), S(422,634), S(490,514),
    S(379,469), S(376,521), S(393,561), S(419,551), S(449,612), S(511,600), S(502,595), S(409,596),
    S(367,496), S(383,515), S(397,543), S(386,593), S(394,617), S(388,642), S(400,622), S(394,551),
    S(392,447), S(386,503), S(398,516), S(388,573), S(383,579), S(394,538), S(400,520), S(380,539),
    S(389,414), S(395,452), S(397,495), S(390,487), S(393,487), S(399,505), S(414,479), S(393,457),
    S(386,416), S(387,442), S(404,432), S(409,431), S(414,428), S(441,343), S(407,362), S(355,390),
    S(387,433), S(397,400), S(403,412), S(410,425), S(419,365), S(368,370), S(319,344), S(345,404),
    };

    tables[BISHOP] = {
    S( 76, 75), S(-19, 93), S(-44,117), S(-28,120), S(-25, 98), S(-94,134), S( 26, 93), S( 76, 93),
    S( 41, 92), S( 60, 88), S( 73, 87), S( 54, 87), S( 60, 97), S( 83, 79), S( 28,107), S( 42, 87),
    S( 65, 75), S( 73,102), S( 83, 98), S( 96, 83), S(145, 71), S(227, 72), S(113, 80), S(121, 75),
    S( 62, 84), S( 71,100), S( 85,102), S(125,108), S( 85,123), S(113, 75), S( 67, 92), S( 57, 84),
    S( 76, 54), S( 73, 89), S( 69,102), S(103,118), S(110, 99), S( 66, 97), S( 66, 71), S( 98, 35),
    S( 63, 63), S( 86, 87), S( 75,105), S( 82, 91), S( 79,119), S( 85, 83), S( 90, 50), S( 99, 62),
    S( 87, 37), S( 76, 51), S( 88, 47), S( 64, 81), S( 82, 77), S( 67, 71), S(110, 62), S( 83, 41),
    S( 55, 52), S( 65, 51), S( 61, 72), S( 47, 73), S( 54, 65), S( 59, 67), S( 26, 75), S( 65, 46),
    };

    tables[KNIGHT] = {
    S(-173,  53), S( -18,  45), S( -70,  80), S(   9,  45), S( 128,  33), S( -35,  58), S(  93,  16), S(-214,  78),
    S( -15,  49), S(  -9,  61), S(  42,  56), S( 103,  46), S( 100,  27), S( 186, -22), S(   3,  44), S(  10,  39),
    S(  -9,  46), S(  40,  51), S(  54, 104), S(  94,  88), S( 171,  55), S( 245,  29), S(  97,  28), S(  20,  41),
    S(  17,  49), S(  22,  63), S(  56,  95), S(  77,  91), S(  47,  87), S( 103,  68), S(  39,  58), S(  74,  41),
    S(   7,  48), S(  23,  41), S(  34,  95), S(  22,  92), S(  37,  88), S(  41,  79), S(  56,  39), S(  19,  50),
    S( -19,  12), S(   1,  35), S(   9,  50), S(  26,  77), S(  38,  67), S(  21,  47), S(  30,  25), S(  -6,  45),
    S( -35,  22), S( -36,  31), S( -17,  36), S(  20,  31), S(  22,  25), S(  -0,  45), S(  18,  45), S(   8,  19),
    S( -70,  12), S(   3,  -7), S( -31,  14), S(   1,  17), S(   5,  22), S(   9,   6), S(   4,   8), S( -22,   3),
    };

    tables[ROOK] = {
    S( 92,173), S(107,178), S(117,174), S(142,164), S(169,161), S(123,175), S(160,159), S(213,131),
    S( 84,185), S( 91,187), S(130,176), S(167,162), S(158,161), S(262,111), S(314,105), S(269,121),
    S( 81,169), S(125,156), S(141,156), S(152,146), S(231,111), S(245,116), S(263,116), S(216,120),
    S( 65,166), S( 79,174), S(116,158), S(132,152), S(128,143), S(165,135), S(112,163), S( 83,148),
    S( 53,146), S( 70,154), S( 77,156), S( 97,149), S( 94,127), S( 80,147), S(119,119), S( 76,113),
    S( 36,125), S( 49,130), S( 63,137), S( 74,125), S( 83,118), S( 94,119), S(132,108), S( 88, 82),
    S( 41,105), S( 48,117), S( 73,124), S( 64,126), S( 73,112), S( 96,108), S( 88, 83), S(  7,112),
    S( 77,116), S( 87,109), S( 95,126), S(101,126), S(112,110), S(105,112), S( 62,117), S( 86, 60),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 127, 184), S(  90, 193), S( 127, 153), S(  92,  99), S(  30, 142), S(  13, 122), S( -90, 176), S(-184, 197),
    S(  25, 122), S(  26, 124), S(  38,  93), S(  58,  42), S(  94,  25), S( 129,  39), S( 102,  65), S(  39,  74),
    S(  21,  84), S(  30,  70), S(  19,  51), S(  39,  20), S(  53,  29), S(  44,  42), S(  52,  52), S(  40,  54),
    S(   8,  62), S(  19,  60), S(  19,  30), S(  16,  36), S(  28,  32), S(  35,  42), S(  56,  39), S(  28,  31),
    S(   1,  60), S(   7,  55), S(  -5,  48), S(  -4,  52), S(  18,  55), S(  19,  56), S(  86,  23), S(  47,  20),
    S(   3,  65), S(   8,  61), S( -11,  55), S(   4,  18), S(  11,  70), S(  71,  54), S( 114,  28), S(  47,  18),
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