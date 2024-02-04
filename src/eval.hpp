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
    S( 88, 46), S( 90, 92), S( 87,108), S( 90,132), S( 91,141), S( 95,150), S( 96,158), S( 99,158),
    S(101,158), S(105,147), S(100,148), S(135, 97), S(140,115), S(304,-35), };

// other tables
constexpr std::array<S, NUM_RANKS> passedPawn = {
    S(  0,  0), S( 78,187), S( 66,121), S( 27, 70), S(  4, 44), S( -9, 19), S(  9,  8), S(  0,  0),};

constexpr std::array<S, NUM_PIECES> pieceVals = {
    S(  0,  0), S(920,812), S(290,190), S(391,279), S(434,484), S( 66, 84), };

// misc terms
constexpr auto doubledPawns = S(-13,-33);
constexpr auto chainedPawns = S( 25, 22);
constexpr auto phalanxPawns = S(  9, 10);

// piece square tables
constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[KING] = {
    S( -39,-178), S( 210,-218), S( -19,  36), S(  87, -86), S( -12,  76), S( 807,-540), S(  48, -21), S(  41, -76),
    S( 798,-520), S(  57,  -9), S(  -9,   4), S( -50,  80), S(  30, -17), S(   3,  34), S( 297,-170), S( 308,-104),
    S(  82, -32), S(  46,  -3), S(   2,  16), S(  25,   9), S(  26,  21), S( 106,  24), S( -48,  76), S( -85,  74),
    S(-135,  51), S(-108,  64), S( -54,  35), S(-115,  73), S(  -1,  30), S( 116,  11), S(-104,  89), S(-220, 100),
    S(-239,  86), S(-183,  92), S(-198,  98), S(-161,  86), S(-193, 108), S(-190, 110), S(-159,  87), S(-247,  84),
    S( -88,  50), S( -85,  53), S(-129,  76), S(-222, 108), S(-274, 130), S(-196, 102), S( -88,  56), S( -89,  29),
    S( -35,  13), S(-156,  71), S( -52,  44), S(-140,  73), S(-138,  70), S(-108,  52), S(   7,  15), S(  48, -30),
    S( -18, -81), S(  -4, -24), S( -23,  -5), S(-135,  11), S(  10, -91), S(-107,  -3), S(  58, -48), S(  52, -85),
    };

    tables[QUEEN] = {
    S(332,549), S(391,562), S(413,560), S(386,606), S(421,604), S(420,599), S(503,514), S(491,508),
    S(347,545), S(344,567), S(393,560), S(364,612), S(346,661), S(509,553), S(408,637), S(504,502),
    S(378,471), S(373,519), S(382,570), S(414,555), S(438,620), S(507,604), S(485,609), S(409,596),
    S(371,491), S(380,518), S(395,546), S(377,603), S(385,628), S(387,649), S(388,636), S(391,537),
    S(387,452), S(381,504), S(391,526), S(384,583), S(374,585), S(387,543), S(395,521), S(370,545),
    S(387,412), S(388,457), S(388,496), S(383,485), S(380,501), S(391,510), S(405,491), S(388,446),
    S(370,433), S(379,439), S(400,424), S(397,444), S(404,428), S(425,350), S(398,372), S(345,367),
    S(377,433), S(370,425), S(378,412), S(392,407), S(396,359), S(345,380), S(274,372), S(386,357),
    };

    tables[BISHOP] = {
    S( 79, 64), S(-19, 98), S(-62,126), S( 27,102), S(-19,104), S(-94,131), S( 71, 70), S( 63, 84),
    S( 58, 72), S( 74, 79), S( 83, 82), S( 56, 90), S( 66, 95), S( 92, 78), S( 35,112), S( 32, 81),
    S( 71, 60), S( 73,101), S(100, 97), S(104, 83), S(139, 77), S(240, 80), S(102, 98), S(127, 73),
    S( 68, 72), S( 71,103), S( 89,105), S(129,113), S( 94,127), S(116, 80), S( 69, 93), S( 62, 85),
    S( 69, 53), S( 77, 90), S( 70,104), S(102,122), S(112,104), S( 73, 97), S( 67, 71), S( 86, 27),
    S( 60, 62), S( 79, 88), S( 68,109), S( 81, 93), S( 72,123), S( 81, 83), S( 88, 31), S( 96, 57),
    S( 79, 36), S( 70, 45), S( 82, 46), S( 59, 71), S( 72, 80), S( 46, 65), S(106, 57), S( 89, 13),
    S( 25, 37), S( 35, 41), S( 44, 37), S( 24, 71), S( 20, 57), S( 41, 50), S(-30, 91), S( 66, 33),
    };

    tables[KNIGHT] = {
    S(-205,  97), S( -51, 100), S( -67, 145), S(  26,  86), S( 181,  70), S( -48, 127), S(  70,  35), S(-206,  59),
    S(  -9,  83), S(   7,  99), S(  62, 112), S( 127, 103), S( 114,  78), S( 207,  32), S( -11,  87), S(  23,  68),
    S(   8,  81), S(  65, 109), S(  93, 123), S( 133, 123), S( 210,  79), S( 286,  55), S( 128,  81), S(  42,  73),
    S(  32,  90), S(  39, 122), S(  81, 134), S( 103, 128), S(  70, 138), S( 144, 101), S(  51, 122), S(  81,  77),
    S(  17,  77), S(  30,  97), S(  55, 140), S(  34, 141), S(  49, 136), S(  52, 128), S(  50, 115), S(  18,  66),
    S(  -9,  49), S(  15,  82), S(  26,  98), S(  46, 121), S(  42, 121), S(  34, 102), S(  44,  76), S( -10,  56),
    S( -36,  58), S( -37,  70), S( -16,  94), S(  21,  82), S(  22,  75), S(   6, 104), S(  23,  73), S(  14,  19),
    S( -59,  -7), S(  -2, -31), S( -40,  49), S( -25,  49), S( -17,  58), S(  -2,  43), S(  -5,   8), S( -27, -55),
    };

    tables[ROOK] = {
    S(176,275), S(193,279), S(216,276), S(234,268), S(270,261), S(192,282), S(226,261), S(256,242),
    S(163,289), S(170,290), S(211,280), S(239,272), S(231,266), S(355,204), S(366,211), S(322,230),
    S(140,277), S(185,262), S(207,260), S(213,250), S(293,220), S(286,227), S(317,216), S(273,218),
    S(121,272), S(132,284), S(175,266), S(185,265), S(182,252), S(225,236), S(156,265), S(127,251),
    S(110,252), S(126,259), S(128,262), S(155,259), S(151,230), S(137,248), S(157,221), S(135,211),
    S( 87,228), S(101,235), S(115,244), S(121,232), S(130,228), S(135,230), S(178,212), S(130,191),
    S( 82,213), S(103,220), S(127,227), S(101,238), S(109,223), S(135,224), S(121,194), S( 49,217),
    S(118,226), S(125,226), S(135,245), S(135,248), S(140,234), S(128,227), S( 91,239), S(116,160),
    };

    tables[PAWN] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 122, 189), S(  76, 206), S( 127, 160), S( 111,  89), S(  70, 129), S(   4, 133), S( -66, 164), S(-191, 201),
    S(  12, 130), S(  25, 124), S(  30, 103), S(  52,  49), S(  89,  30), S( 134,  38), S(  97,  68), S(  33,  75),
    S(  18,  84), S(  30,  70), S(  19,  52), S(  40,  20), S(  52,  30), S(  45,  43), S(  58,  47), S(  42,  52),
    S(   0,  63), S(  19,  60), S(  14,  32), S(  17,  35), S(  24,  32), S(  31,  41), S(  54,  38), S(  27,  29),
    S( -10,  65), S(   4,  56), S(  -9,  49), S(  -5,  50), S(  12,  60), S(   8,  60), S(  83,  24), S(  43,  20),
    S( -12,  70), S(   5,  64), S( -13,  57), S(  -8,  20), S(   8,  67), S(  63,  58), S( 115,  27), S(  42,  18),
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