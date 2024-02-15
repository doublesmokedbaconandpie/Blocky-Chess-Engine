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
inline constexpr std::array<S, 9> knightMobility = {
    S( 26,-69), S( 40, 50), S( 49, 98), S( 56,122), S( 61,138), S( 64,148), S( 71,140), S( 80,121),
    S(139, 36), };

inline constexpr std::array<S, 14> bishopMobility = {
    S( 68, 34), S( 77, 80), S( 85,102), S( 91,122), S( 99,140), S(104,156), S(108,158), S(113,156),
    S(117,158), S(134,142), S(133,144), S(200, 83), S(174,118), S(405,-59), };

inline constexpr std::array<S, 15> rookMobility = {
    S(119,178), S(131,203), S(137,209), S(143,214), S(143,229), S(152,229), S(158,240), S(167,239),
    S(176,241), S(182,245), S(197,245), S(201,252), S(205,248), S(224,237), S(203,247), };

// other tables
inline constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 76,185), S( 60,125), S( 29, 69), S( -1, 43), S( -8, 18), S(  7,  8), S(  0,  0),
    };

inline constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(942,802), S(300,184), S(360,190), S(359,331), S( 70, 82), };

// misc terms
inline constexpr auto doubledPawns = S(-16,-32);
inline constexpr auto chainedPawns = S( 23, 21);
inline constexpr auto phalanxPawns = S( 12, 11);
inline constexpr int tempo = 11;

// piece square tables
inline constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S(  33,-216), S( 159,-176), S( -39,  22), S(  19, -41), S(  65,  22), S( 714,-481), S(  68, -20), S(  86, -78),
    S( 711,-478), S( -41,  34), S( -68,  34), S( 225, -73), S(  33,   4), S(  39,  40), S( 221,-106), S( 191, -52),
    S(  37, -10), S(  58,  11), S( -23,  31), S( -46,  46), S(  14,  28), S( 103,  32), S(  67,  23), S(  29,  41),
    S(-158,  61), S( -83,  55), S( -69,  46), S( -74,  64), S( -16,  39), S( 125,  17), S( -23,  55), S(-115,  58),
    S(-178,  59), S(-187,  85), S(-194, 102), S(-162,  91), S(-153,  95), S(-170, 101), S(-143,  82), S(-208,  64),
    S( -73,  42), S(-104,  60), S(-105,  69), S(-227, 112), S(-262, 123), S(-190,  96), S( -89,  47), S( -99,  26),
    S( -46,  10), S(-144,  51), S( -71,  47), S(-142,  66), S(-137,  64), S(-110,  47), S(   2,  10), S(  35, -32),
    S( -32, -85), S(  -7, -31), S( -24,  -5), S(-134,   8), S(  10, -82), S( -89,  -8), S(  57, -52), S(  45, -88),
    };

    tables[QUEEN] = {
    S(338,552), S(370,572), S(403,571), S(385,605), S(406,615), S(436,581), S(526,505), S(478,522),
    S(343,548), S(348,564), S(397,562), S(372,607), S(346,663), S(488,567), S(411,638), S(469,536),
    S(377,475), S(373,523), S(393,558), S(417,551), S(452,605), S(514,596), S(503,591), S(410,598),
    S(366,493), S(383,514), S(391,549), S(385,598), S(395,615), S(391,637), S(395,623), S(396,551),
    S(389,454), S(387,499), S(398,516), S(386,572), S(381,582), S(398,531), S(402,519), S(379,548),
    S(388,431), S(395,453), S(394,501), S(390,489), S(396,481), S(401,501), S(412,480), S(399,441),
    S(382,423), S(390,443), S(404,434), S(409,432), S(414,426), S(441,351), S(404,367), S(342,414),
    S(382,442), S(396,395), S(405,408), S(408,429), S(420,356), S(356,385), S(341,301), S(323,442),
    };

    tables[BISHOP] = {
    S(  69,  81), S(   2,  91), S( -47, 122), S( -48, 127), S( -41, 103), S(-100, 135), S( -36, 110), S( 103,  89),
    S(  42,  86), S(  60,  87), S(  74,  90), S(  61,  89), S(  55,  98), S(  97,  69), S(  22, 110), S(  47,  82),
    S(  65,  76), S(  78,  99), S(  86,  95), S( 100,  82), S( 144,  73), S( 223,  75), S( 111,  79), S( 124,  69),
    S(  57,  85), S(  72,  98), S(  88, 101), S( 125, 108), S(  83, 123), S( 104,  80), S(  65,  94), S(  56,  83),
    S(  77,  53), S(  75,  90), S(  70, 104), S( 103, 119), S( 109,  99), S(  65,  99), S(  64,  76), S(  97,  34),
    S(  63,  66), S(  87,  87), S(  73, 107), S(  82,  92), S(  79, 119), S(  87,  82), S(  90,  50), S( 101,  59),
    S(  84,  42), S(  74,  51), S(  90,  48), S(  62,  84), S(  83,  77), S(  76,  68), S( 111,  61), S(  85,  36),
    S(  52,  49), S(  70,  51), S(  60,  74), S(  51,  70), S(  52,  67), S(  59,  67), S(  18,  82), S(  60,  49),
    };

    tables[KNIGHT] = {
    S(-164,  57), S( -22,  49), S( -71,  74), S(  45,  30), S( 124,  30), S( -27,  54), S(  82,  18), S(-165,  41),
    S(  -5,  53), S(  -4,  53), S(  42,  59), S(  91,  52), S(  94,  31), S( 183, -17), S(   6,  32), S(  -9,  59),
    S(  -9,  44), S(  43,  51), S(  51, 106), S(  92,  90), S( 167,  56), S( 248,  28), S( 107,  22), S(  20,  37),
    S(  14,  52), S(  20,  67), S(  53,  97), S(  78,  90), S(  46,  87), S( 103,  72), S(  38,  58), S(  71,  42),
    S(   6,  54), S(  19,  41), S(  32,  99), S(  19,  93), S(  37,  90), S(  42,  81), S(  53,  46), S(  18,  55),
    S( -17,  14), S(  -2,  37), S(   7,  52), S(  24,  78), S(  38,  67), S(  20,  47), S(  30,  27), S(  -5,  46),
    S( -32,  17), S( -42,  28), S( -17,  29), S(  18,  32), S(  22,  25), S(   2,  45), S(  18,  43), S(  12,  15),
    S( -63,   5), S(   1,  -5), S( -36,  16), S(  -3,  21), S(   7,  29), S(  11,   5), S(   6,   7), S( -21,   9),
    };

    tables[ROOK] = {
    S( 95,170), S(110,175), S(111,179), S(139,164), S(163,161), S(129,175), S(193,146), S(208,135),
    S( 84,185), S( 83,192), S(137,173), S(164,165), S(157,162), S(248,115), S(300,110), S(266,125),
    S( 76,173), S(123,159), S(141,157), S(158,146), S(229,107), S(250,114), S(260,117), S(215,118),
    S( 68,166), S( 79,174), S(112,162), S(126,154), S(125,147), S(159,140), S(120,160), S( 86,148),
    S( 53,147), S( 69,155), S( 74,160), S( 91,153), S( 94,129), S( 78,150), S(118,122), S( 70,119),
    S( 33,129), S( 49,135), S( 62,141), S( 77,122), S( 83,121), S( 93,118), S(135,107), S( 93, 77),
    S( 42,105), S( 51,116), S( 67,131), S( 63,128), S( 73,113), S( 95,108), S( 86, 86), S( 22,102),
    S( 74,118), S( 86,111), S( 93,128), S( 98,128), S(111,113), S(104,113), S( 62,112), S( 83, 64),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 126, 184), S(  81, 199), S( 119, 155), S(  94, 100), S(  38, 138), S(  27, 109), S( -75, 171), S(-166, 189),
    S(  22, 125), S(  26, 126), S(  37,  95), S(  57,  42), S(  98,  19), S( 129,  41), S( 101,  65), S(  40,  76),
    S(  19,  86), S(  31,  71), S(  18,  53), S(  38,  19), S(  52,  28), S(  43,  44), S(  50,  55), S(  39,  55),
    S(   9,  63), S(  20,  60), S(  19,  31), S(  16,  36), S(  27,  33), S(  35,  42), S(  55,  39), S(  28,  31),
    S(   0,  61), S(   8,  54), S(  -7,  49), S(  -4,  53), S(  18,  56), S(  17,  57), S(  84,  23), S(  46,  20),
    S(   2,  66), S(   8,  61), S( -12,  57), S(   4,  16), S(  11,  71), S(  71,  55), S( 114,  27), S(  47,  19),
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