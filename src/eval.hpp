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
    S( 46,-55), S( 59, 54), S( 69,103), S( 76,126), S( 81,138), S( 84,149), S( 90,140), S(102,119),
    S(158, 35), };

inline constexpr std::array<S, 14> bishopMobility = {
    S( 55, 32), S( 64, 78), S( 72, 96), S( 78,117), S( 86,134), S( 91,150), S( 94,153), S(100,150),
    S(103,154), S(119,136), S(118,139), S(184, 77), S(175,104), S(395,-65), };

inline constexpr std::array<S, 15> rookMobility = {
    S(120,188), S(132,203), S(139,207), S(145,214), S(145,228), S(155,230), S(159,240), S(169,239),
    S(178,241), S(184,245), S(198,245), S(205,251), S(205,249), S(223,239), S(207,246), };

// other tables
inline constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 73,185), S( 59,127), S( 29, 70), S(  1, 43), S( -7, 18), S(  8,  8), S(  0,  0),
    };

inline constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(963,790), S(308,181), S(333,188), S(357,330), S( 71, 81), };

// misc terms
inline constexpr auto doubledPawns = S(-19,-31);
inline constexpr auto chainedPawns = S( 23, 22);
inline constexpr auto phalanxPawns = S( 12, 11);
inline constexpr auto bishopPair = S( 19, 85);
inline constexpr int tempo = 11;

// piece square tables
inline constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S(  41,-181), S( 215,-191), S( -20,  27), S(  92, -67), S(  33,  27), S( 724,-487), S(  93, -26), S( -20, -46),
    S( 726,-493), S( -62,  50), S( -39,  30), S( 114, -35), S(   9,  10), S(  47,  38), S( 193, -99), S( 225, -75),
    S(   5,  -8), S(  17,  24), S( -13,  27), S( -23,  39), S(  32,  21), S(  97,  27), S(  80,  17), S(  65,  21),
    S(-130,  48), S(-135,  74), S( -84,  55), S( -30,  47), S( -10,  36), S( 133,  12), S( -16,  54), S(-124,  63),
    S(-197,  67), S(-210,  95), S(-208, 102), S(-150,  89), S(-156,  96), S(-155,  94), S(-149,  84), S(-231,  73),
    S( -85,  40), S( -93,  51), S(-103,  68), S(-216, 108), S(-259, 123), S(-183,  94), S( -82,  46), S( -92,  24),
    S( -57,  18), S(-145,  57), S( -72,  48), S(-144,  66), S(-135,  63), S(-111,  48), S(   2,  12), S(  36, -32),
    S( -31, -71), S( -11, -25), S( -23,  -4), S(-131,   9), S(  10, -81), S( -88,  -9), S(  58, -52), S(  47, -90),
    };

    tables[QUEEN] = {
    S(343,539), S(381,562), S(406,574), S(382,608), S(415,605), S(430,587), S(521,504), S(502,500),
    S(350,540), S(350,568), S(403,557), S(379,603), S(359,656), S(512,545), S(429,631), S(496,510),
    S(383,467), S(381,518), S(397,558), S(423,548), S(453,611), S(515,597), S(507,592), S(414,593),
    S(371,494), S(388,513), S(403,540), S(391,588), S(398,614), S(392,639), S(406,618), S(399,548),
    S(397,444), S(391,501), S(403,512), S(392,572), S(388,575), S(399,534), S(405,517), S(385,536),
    S(394,411), S(400,450), S(401,494), S(396,483), S(397,484), S(404,501), S(419,476), S(397,456),
    S(392,412), S(392,441), S(409,431), S(414,428), S(419,426), S(446,340), S(412,358), S(360,388),
    S(390,433), S(402,399), S(408,411), S(415,423), S(424,364), S(373,367), S(325,337), S(351,400),
    };

    tables[BISHOP] = {
    S( 69, 72), S(-25, 86), S(-42,108), S(-25,108), S(-19, 89), S(-93,126), S( 25, 87), S( 76, 84),
    S( 29, 87), S( 49, 85), S( 60, 82), S( 45, 83), S( 49, 92), S( 75, 77), S( 21, 99), S( 30, 85),
    S( 54, 71), S( 61, 98), S( 72, 98), S( 84, 79), S(134, 69), S(219, 67), S(106, 76), S(110, 69),
    S( 52, 78), S( 59, 98), S( 73, 99), S(114,107), S( 73,120), S(102, 74), S( 54, 86), S( 45, 81),
    S( 64, 50), S( 61, 86), S( 57,101), S( 91,116), S( 99, 98), S( 53, 93), S( 55, 68), S( 85, 29),
    S( 51, 57), S( 74, 85), S( 63,102), S( 70, 90), S( 66,116), S( 73, 82), S( 78, 47), S( 88, 58),
    S( 76, 32), S( 64, 45), S( 77, 45), S( 52, 76), S( 70, 74), S( 55, 69), S( 99, 60), S( 72, 36),
    S( 41, 46), S( 54, 48), S( 49, 64), S( 36, 67), S( 44, 59), S( 47, 64), S( 22, 68), S( 53, 39),
    };

    tables[KNIGHT] = {
    S(-161,  60), S(  -9,  46), S( -61,  82), S(  11,  47), S( 140,  32), S( -25,  58), S(  91,  19), S(-192,  73),
    S(  -6,  51), S(   3,  61), S(  53,  57), S( 113,  46), S( 109,  28), S( 193, -22), S(   8,  45), S(  24,  36),
    S(  -1,  47), S(  50,  53), S(  63, 106), S( 104,  89), S( 180,  55), S( 253,  31), S( 107,  28), S(  31,  40),
    S(  28,  50), S(  32,  65), S(  65,  96), S(  87,  92), S(  58,  89), S( 112,  69), S(  50,  59), S(  84,  40),
    S(  18,  47), S(  34,  41), S(  45,  96), S(  32,  93), S(  48,  89), S(  51,  79), S(  67,  39), S(  30,  48),
    S(  -9,  12), S(  11,  38), S(  19,  54), S(  36,  79), S(  48,  68), S(  32,  50), S(  41,  25), S(   4,  44),
    S( -25,  23), S( -26,  33), S(  -7,  38), S(  30,  34), S(  32,  27), S(   9,  47), S(  28,  43), S(  19,  18),
    S( -63,  12), S(  14,  -5), S( -21,  16), S(  11,  18), S(  16,  23), S(  20,   5), S(  15,   9), S( -12,  -2),
    };

    tables[ROOK] = {
    S( 92,175), S(106,181), S(116,177), S(140,167), S(167,164), S(121,179), S(153,165), S(211,134),
    S( 85,187), S( 93,189), S(131,178), S(166,164), S(158,163), S(263,113), S(313,108), S(266,125),
    S( 82,171), S(126,157), S(141,158), S(153,147), S(231,114), S(245,118), S(263,119), S(216,122),
    S( 67,168), S( 81,175), S(117,159), S(132,154), S(128,144), S(167,136), S(113,165), S( 84,150),
    S( 55,148), S( 71,156), S( 78,158), S( 99,150), S( 95,128), S( 82,148), S(121,120), S( 78,115),
    S( 38,126), S( 50,132), S( 65,138), S( 75,127), S( 84,120), S( 95,121), S(134,109), S( 89, 85),
    S( 42,107), S( 49,119), S( 75,126), S( 66,127), S( 74,113), S( 97,109), S( 91, 85), S( 10,113),
    S( 78,118), S( 89,111), S( 97,129), S(102,128), S(114,112), S(106,114), S( 63,119), S( 87, 61),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 128, 184), S(  93, 192), S( 127, 153), S(  93,  99), S(  31, 141), S(   8, 124), S( -94, 176), S(-186, 197),
    S(  26, 123), S(  27, 124), S(  39,  93), S(  58,  42), S(  94,  25), S( 129,  38), S( 103,  64), S(  39,  73),
    S(  21,  84), S(  30,  71), S(  19,  51), S(  39,  20), S(  53,  29), S(  44,  41), S(  52,  52), S(  41,  54),
    S(   9,  62), S(  19,  61), S(  19,  30), S(  16,  37), S(  28,  33), S(  35,  42), S(  56,  39), S(  28,  31),
    S(   1,  60), S(   7,  55), S(  -5,  48), S(  -4,  52), S(  18,  55), S(  19,  56), S(  86,  23), S(  47,  20),
    S(   3,  65), S(   8,  62), S( -11,  56), S(   4,  19), S(  11,  69), S(  71,  54), S( 114,  28), S(  47,  18),
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