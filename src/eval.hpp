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
    S( 65, 33), S( 73, 86), S( 79,105), S( 85,126), S( 92,142), S( 97,157), S( 99,159), S(105,157),
    S(106,158), S(123,141), S(121,144), S(184, 81), S(171,112), S(372,-53), };

// other tables
constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 79,187), S( 66,120), S( 27, 70), S(  4, 44), S( -9, 20), S(  9,  7), S(  0,  0),};

constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(919,812), S(296,189), S(393,279), S(437,483), S( 67, 84), };

// misc terms
constexpr auto doubledPawns = S(-13,-33);
constexpr auto chainedPawns = S( 24, 21);
constexpr auto phalanxPawns = S( 11,  9);

// piece square tables
constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S( -36,-180), S( 206,-215), S( -23,  38), S(  83, -84), S(  -7,  74), S( 807,-539), S(  52, -23), S(  42, -76),
    S( 803,-522), S(  66, -11), S( -20,   8), S( -49,  80), S(  30, -16), S(  -2,  35), S( 291,-166), S( 312,-106),
    S(  92, -37), S(  51,  -4), S(   1,  16), S(  24,   9), S(  18,  24), S( 103,  26), S( -46,  76), S( -76,  72),
    S(-129,  49), S(-105,  63), S( -58,  38), S(-122,  76), S(  -3,  31), S( 122,   9), S(-104,  89), S(-217,  99),
    S(-237,  84), S(-176,  89), S(-197,  97), S(-162,  87), S(-193, 109), S(-189, 109), S(-158,  87), S(-247,  84),
    S( -85,  48), S( -82,  52), S(-127,  75), S(-219, 107), S(-273, 129), S(-197, 103), S( -89,  56), S( -88,  29),
    S( -35,  13), S(-154,  71), S( -53,  45), S(-138,  73), S(-136,  70), S(-108,  52), S(   7,  15), S(  47, -30),
    S( -19, -82), S(  -4, -24), S( -25,  -3), S(-137,  12), S(   6, -89), S(-106,  -4), S(  56, -48), S(  53, -85),
    };

    tables[QUEEN] = {
    S(332,551), S(392,563), S(414,560), S(387,607), S(422,605), S(420,600), S(505,515), S(491,509),
    S(350,544), S(344,569), S(395,562), S(365,612), S(347,661), S(510,554), S(408,639), S(506,502),
    S(379,472), S(375,519), S(385,568), S(414,557), S(436,623), S(508,604), S(487,608), S(411,594),
    S(372,490), S(381,519), S(396,544), S(380,603), S(388,625), S(386,650), S(391,635), S(392,538),
    S(387,452), S(383,502), S(391,528), S(385,582), S(376,586), S(389,542), S(395,523), S(371,545),
    S(387,412), S(387,458), S(390,494), S(384,487), S(384,498), S(394,510), S(406,492), S(390,446),
    S(371,431), S(381,439), S(400,426), S(400,441), S(405,431), S(426,352), S(402,369), S(346,365),
    S(377,434), S(372,424), S(378,415), S(391,410), S(396,360), S(346,381), S(279,369), S(386,360),
    };

    tables[BISHOP] = {
    S( 75, 65), S(-20, 93), S(-63,123), S( 25, 96), S(-25, 98), S(-86,124), S( 66, 69), S( 63, 87),
    S( 59, 69), S( 63, 79), S( 74, 80), S( 48, 89), S( 58, 94), S( 83, 77), S( 25,113), S( 38, 79),
    S( 68, 59), S( 68, 99), S( 87, 98), S( 91, 84), S(124, 79), S(228, 81), S( 94, 98), S(121, 73),
    S( 64, 72), S( 68,100), S( 81,103), S(122,110), S( 83,125), S(107, 79), S( 64, 91), S( 59, 82),
    S( 69, 51), S( 72, 88), S( 66,103), S(100,119), S(109, 99), S( 66, 97), S( 63, 70), S( 92, 25),
    S( 60, 65), S( 80, 90), S( 67,107), S( 80, 91), S( 74,122), S( 81, 82), S( 89, 39), S( 97, 62),
    S( 79, 42), S( 73, 46), S( 82, 47), S( 60, 71), S( 75, 77), S( 48, 67), S(109, 59), S( 92, 23),
    S( 32, 45), S( 39, 44), S( 48, 43), S( 20, 74), S( 23, 63), S( 48, 57), S(-27, 96), S( 73, 39),
    };

    tables[KNIGHT] = {
    S(-207,  94), S( -50, 100), S( -63, 144), S(  26,  87), S( 182,  70), S( -48, 127), S(  70,  35), S(-205,  55),
    S(  -7,  82), S(   7,  99), S(  64, 111), S( 126, 104), S( 114,  79), S( 205,  33), S(  -5,  85), S(  23,  68),
    S(   6,  82), S(  65, 110), S(  95, 123), S( 133, 123), S( 210,  79), S( 287,  54), S( 125,  83), S(  42,  73),
    S(  32,  90), S(  39, 123), S(  80, 136), S( 102, 129), S(  70, 138), S( 142, 102), S(  51, 122), S(  82,  76),
    S(  16,  77), S(  31,  97), S(  55, 139), S(  34, 142), S(  51, 137), S(  54, 127), S(  50, 116), S(  17,  66),
    S( -10,  49), S(  13,  83), S(  25,  98), S(  46, 121), S(  45, 119), S(  36, 101), S(  43,  77), S( -11,  56),
    S( -36,  57), S( -36,  69), S( -17,  94), S(  25,  80), S(  23,  76), S(   6, 104), S(  23,  73), S(  12,  21),
    S( -62,  -5), S(  -5, -30), S( -39,  49), S( -26,  50), S( -18,  60), S(   0,  42), S(  -9,  12), S( -27, -53),
    };

    tables[ROOK] = {
    S(177,275), S(195,279), S(220,274), S(237,267), S(272,260), S(195,281), S(229,259), S(256,242),
    S(165,289), S(172,290), S(212,279), S(240,271), S(231,266), S(357,204), S(371,209), S(325,228),
    S(141,277), S(187,262), S(208,261), S(215,250), S(294,220), S(287,226), S(323,214), S(276,217),
    S(122,272), S(133,284), S(175,266), S(187,265), S(184,251), S(225,236), S(158,264), S(128,251),
    S(111,252), S(126,259), S(130,262), S(156,258), S(154,230), S(140,247), S(160,220), S(136,211),
    S( 88,229), S(103,234), S(117,243), S(124,232), S(134,227), S(139,228), S(179,211), S(132,191),
    S( 84,213), S(106,220), S(128,227), S(106,238), S(112,223), S(136,224), S(124,194), S( 50,217),
    S(118,226), S(126,226), S(137,245), S(136,247), S(140,234), S(130,226), S( 91,239), S(116,162),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 123, 188), S(  76, 207), S( 128, 160), S( 111,  90), S(  71, 129), S(   0, 134), S( -71, 168), S(-189, 200),
    S(  12, 130), S(  27, 124), S(  29, 104), S(  54,  49), S(  88,  31), S( 135,  37), S(  96,  70), S(  33,  76),
    S(  19,  83), S(  31,  71), S(  19,  53), S(  40,  20), S(  51,  30), S(  45,  44), S(  56,  48), S(  43,  52),
    S(   1,  63), S(  19,  60), S(  16,  32), S(  17,  36), S(  25,  33), S(  33,  40), S(  53,  40), S(  28,  28),
    S(  -8,  64), S(   5,  56), S(  -8,  49), S(  -2,  48), S(  16,  58), S(  10,  59), S(  84,  24), S(  45,  19),
    S( -13,  70), S(   6,  64), S( -14,  58), S(  -2,  22), S(   8,  69), S(  63,  59), S( 115,  28), S(  42,  17),
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