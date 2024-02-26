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

#include "pieceSets.hpp"
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
    S( 49,-71), S( 63, 47), S( 71, 98), S( 78,122), S( 84,136), S( 87,146), S( 94,137), S(105,116),
    S(157, 34), };

inline constexpr std::array<S, 14> bishopMobility = {
    S( 64, 25), S( 70, 65), S( 78, 92), S( 84,114), S( 92,131), S( 97,146), S(100,150), S(105,149),
    S(109,153), S(125,135), S(130,136), S(189, 78), S(188,102), S(425,-81), };

inline constexpr std::array<S, 15> rookMobility = {
    S(139,178), S(149,199), S(155,205), S(161,212), S(160,226), S(169,231), S(176,239), S(185,238),
    S(193,240), S(199,245), S(212,246), S(218,252), S(222,249), S(237,240), S(235,239), };

// other tables
inline constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 91,176), S( 81,119), S( 35, 68), S(  2, 42), S( -7, 20), S( 10, 11), S(  0,  0),
    };

inline constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(1062,789), S(341,179), S(370,188), S(397,336), S( 79, 79), };

// misc terms
inline constexpr auto doubledPawns = S(-24,-28);
inline constexpr auto chainedPawns = S( 24, 22);
inline constexpr auto phalanxPawns = S( 13, 14);
inline constexpr auto bishopPair = S( 25, 88);
inline constexpr int tempo = 11;

// piece square tables
inline constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S(  15,-127), S(  24, -77), S( -24,   3), S(  24, -46), S( -58,  57), S( 542,-331), S( 163, -39), S( -59,  16),
    S( 394,-346), S(-181,  86), S(-114,  81), S(  33,  -0), S(-109,  82), S( 144,   0), S( 108,   2), S( 371,-137),
    S(-150,  64), S( -59,  47), S(  -7,  20), S( -33,  41), S(   4,  28), S( 143,  17), S( 137, -10), S( 142,  -4),
    S(-163,  61), S(-109,  63), S( -77,  47), S(  33,  17), S(  -7,  27), S( 139,  13), S(  45,  39), S( -75,  48),
    S(-201,  69), S(-190,  84), S(-174,  87), S(-113,  67), S(-107,  74), S(-122,  78), S(-130,  74), S(-174,  52),
    S( -82,  35), S( -75,  40), S( -61,  48), S(-172,  84), S(-204,  99), S(-159,  78), S( -67,  30), S( -69,   5),
    S( -44,  15), S( -94,  30), S( -55,  34), S(-120,  49), S(-107,  42), S( -89,  33), S(  31,  -8), S(  56, -49),
    S( -39, -64), S(   7, -42), S( -10, -13), S(-127,   3), S(  30, -96), S( -64, -26), S(  79, -67), S(  67,-106),
    };

    tables[QUEEN] = {
    S(344,551), S(390,555), S(405,570), S(400,595), S(430,583), S(484,553), S(546,491), S(511,488),
    S(352,538), S(356,565), S(419,544), S(404,575), S(373,639), S(525,520), S(441,612), S(508,501),
    S(391,463), S(389,510), S(410,536), S(430,544), S(469,588), S(535,578), S(538,549), S(428,574),
    S(377,494), S(394,512), S(411,524), S(401,576), S(400,611), S(395,635), S(423,588), S(404,544),
    S(403,441), S(400,489), S(416,499), S(404,549), S(397,559), S(407,521), S(413,505), S(396,529),
    S(407,395), S(409,439), S(410,478), S(408,465), S(405,471), S(412,487), S(431,450), S(397,451),
    S(403,394), S(400,434), S(417,419), S(422,418), S(428,415), S(462,317), S(422,346), S(359,398),
    S(399,429), S(419,378), S(414,402), S(423,415), S(431,359), S(391,338), S(351,346), S(329,472),
    };

    tables[BISHOP] = {
    S( 56, 73), S(-25, 88), S(-41, 96), S(-27,104), S(-34, 97), S(-56,105), S(157, 62), S(129, 61),
    S( 11, 95), S( 47, 84), S( 59, 82), S( 36, 89), S( 55, 83), S( 76, 72), S(  5, 97), S( 30, 84),
    S( 55, 70), S( 60, 96), S( 70, 94), S( 92, 73), S(144, 59), S(219, 59), S(110, 73), S(110, 62),
    S( 47, 81), S( 60, 96), S( 73, 93), S(117,102), S( 72,116), S( 99, 77), S( 51, 88), S( 52, 69),
    S( 64, 52), S( 59, 81), S( 58, 97), S( 86,115), S( 93, 95), S( 51, 90), S( 61, 60), S( 85, 32),
    S( 48, 57), S( 81, 79), S( 63, 96), S( 67, 86), S( 66,107), S( 69, 80), S( 80, 48), S( 89, 54),
    S( 76, 37), S( 60, 49), S( 82, 44), S( 50, 75), S( 70, 69), S( 72, 60), S( 95, 59), S( 75, 19),
    S( 44, 52), S( 69, 52), S( 46, 66), S( 34, 67), S( 54, 54), S( 48, 68), S( 47, 62), S( 38, 67),
    };

    tables[KNIGHT] = {
    S(-165,  41), S( -35,  58), S( -42,  68), S(  28,  53), S( 143,  23), S(  -7,  54), S( 123,   3), S(-198,  93),
    S(  -0,  50), S(  -2,  67), S(  55,  54), S( 119,  41), S(  97,  41), S( 187, -20), S(  21,  38), S(  11,  48),
    S(  -9,  48), S(  46,  48), S(  72, 106), S( 111,  82), S( 206,  44), S( 267,  21), S( 113,  31), S(  26,  40),
    S(  26,  50), S(  32,  59), S(  68,  93), S(  88,  90), S(  55,  89), S( 106,  72), S(  51,  56), S(  90,  33),
    S(  15,  45), S(  38,  46), S(  48,  91), S(  33,  90), S(  50,  87), S(  52,  77), S(  72,  39), S(  27,  50),
    S(  -5,   4), S(  11,  37), S(  19,  51), S(  29,  81), S(  51,  61), S(  31,  46), S(  41,  20), S(   6,  43),
    S( -35,  23), S( -30,  37), S(  -1,  33), S(  30,  34), S(  29,  21), S(  21,  39), S(  22,  35), S(  20,  25),
    S( -46,  26), S(  10,   4), S( -24,   7), S(  16,  21), S(  14,  20), S(  21,   7), S(  13,   9), S( -14,   3),
    };

    tables[ROOK] = {
    S(104,171), S( 99,182), S(111,177), S(144,163), S(155,168), S(110,182), S(152,164), S(195,143),
    S( 91,178), S(100,178), S(139,171), S(179,151), S(177,147), S(260,108), S(314,102), S(273,110),
    S( 96,159), S(128,151), S(146,150), S(158,141), S(233,104), S(271,101), S(273,102), S(211,119),
    S( 79,157), S( 83,166), S(125,147), S(142,141), S(131,136), S(164,132), S(116,154), S( 82,147),
    S( 61,137), S( 65,150), S( 83,144), S(102,139), S( 91,123), S( 77,146), S(117,120), S( 64,113),
    S( 34,122), S( 49,125), S( 69,127), S( 78,119), S( 85,106), S( 95,108), S(127,102), S( 83, 81),
    S( 46, 96), S( 50,109), S( 76,117), S( 74,114), S( 78,100), S( 98, 99), S( 94, 77), S( 23,106),
    S( 83,106), S( 92,102), S(100,118), S(107,116), S(115,100), S(107,104), S( 68,108), S( 91, 60),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 165, 163), S( 120, 172), S( 138, 142), S(  98, 102), S(  48, 122), S(  31, 113), S(-129, 188), S(-210, 206),
    S(  33, 114), S(  30, 118), S(  46,  82), S(  47,  45), S(  95,  13), S( 143,  34), S( 113,  57), S(  49,  78),
    S(  22,  81), S(  32,  67), S(  21,  51), S(  44,  19), S(  62,  29), S(  47,  38), S(  53,  54), S(  39,  56),
    S(  11,  60), S(  22,  58), S(  25,  28), S(  21,  36), S(  33,  33), S(  40,  43), S(  59,  39), S(  27,  32),
    S(   4,  55), S(  10,  52), S(  -1,  45), S(  -0,  51), S(  20,  53), S(  27,  52), S(  89,  21), S(  47,  23),
    S(   7,  62), S(   9,  59), S(  -9,  53), S(   7,  22), S(  12,  67), S(  75,  52), S( 112,  27), S(  46,  23),
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