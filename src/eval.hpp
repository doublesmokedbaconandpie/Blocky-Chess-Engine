#pragma once

#include "move.hpp"
#include "bitboard.hpp"
#include "types.hpp"

namespace Eval {

constexpr int totalPhase = 24;
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
S evalPassedPawns(const PieceSets& pieceSets, bool isWhiteTurn);
int mopUpScore(const PieceSets& pieceSets, int eval);
int getPiecePhase(pieceTypes piece);
S getPSQTVal(Square square, pieceTypes currPiece);

// bitmasking tricks
bool isPassedPawn(Square pawn, uint64_t enemyPawns, bool isWhitePawn);

/*************
 * Evaluation Terms
**************/

constexpr std::array<S, NUM_RANKS> passedPawn = {S(  0,  0), S( 69,186), S( 55,107), S( 20, 63), S( -1, 34), S(-21, 11), S(  3, -4), S(  0,  0), };
constexpr std::array<S, NUM_PIECES> pieceVals = {S(  0,  0), S(896,785), S(333,271), S(384,274), S(417,470), S( 69, 90), };

constexpr auto PSQT = [] {
    std::array<std::array<S, BOARD_SIZE>, 6> tables{};

    tables[WKing] = {
    S(  32,-248), S( 376,-280), S( -11,  46), S(-125, 117), S( 238, -65), S( 954,-587), S(-153, 114), S(  16,  -6),
    S( 895,-539), S(  57,   6), S(  74, -51), S( 205, -64), S(  59, -65), S(  -9,  35), S( 262,-178), S( 293, -83),
    S( 189, -77), S(  26,  11), S(-155, 104), S( -82,  62), S(  13,  22), S( 148,  -3), S( -20,  66), S( -92,  72),
    S(-142,  52), S(-232, 122), S(  33,   4), S(-161,  88), S( -68,  75), S( 180, -15), S(-111,  82), S(-219,  93),
    S(-364, 125), S(-227, 101), S(-193,  97), S(-173,  97), S(-184, 103), S(-184, 110), S(-153,  83), S(-281, 100),
    S(-123,  72), S(-163,  91), S(-147,  80), S(-241, 113), S(-268, 125), S(-216, 113), S( -84,  58), S(-109,  44),
    S( -26,  -2), S(-158,  77), S( -64,  58), S(-135,  75), S(-134,  72), S(-119,  59), S( -14,  27), S(  37, -21),
    S(  -8, -73), S( -10, -18), S( -31,   9), S(-142,  19), S(   3, -83), S(-113,   0), S(  48, -38), S(  44, -72),
    };

    tables[WQueen] = {
    S( 353, 514), S( 420, 530), S( 428, 535), S( 408, 568), S( 426, 586), S( 407, 613), S( 507, 502), S( 506, 506),
    S( 348, 522), S( 351, 547), S( 392, 542), S( 353, 606), S( 342, 653), S( 517, 536), S( 384, 651), S( 504, 501),
    S( 386, 440), S( 375, 490), S( 366, 581), S( 408, 546), S( 429, 617), S( 497, 601), S( 485, 588), S( 403, 597),
    S( 368, 484), S( 377, 498), S( 376, 552), S( 373, 590), S( 384, 608), S( 394, 641), S( 372, 638), S( 392, 531),
    S( 384, 434), S( 375, 493), S( 381, 520), S( 374, 588), S( 366, 576), S( 381, 535), S( 388, 518), S( 359, 546),
    S( 383, 410), S( 382, 445), S( 377, 498), S( 370, 495), S( 372, 496), S( 382, 516), S( 397, 492), S( 382, 428),
    S( 355, 442), S( 376, 441), S( 392, 418), S( 389, 440), S( 395, 425), S( 411, 358), S( 403, 349), S( 359, 342),
    S( 368, 456), S( 356, 420), S( 372, 390), S( 386, 397), S( 388, 375), S( 313, 401), S( 325, 239), S( 345, 404),
    };

    tables[WBishop] = {
    S( 100, 116), S(  32, 145), S( -27, 181), S(  41, 179), S( -53, 191), S(-109, 214), S( -27, 156), S( 133, 108),
    S( 106, 105), S( 121, 133), S( 128, 152), S(  94, 157), S( 105, 163), S( 160, 130), S(  88, 160), S(  76, 134),
    S( 107, 109), S( 113, 162), S( 148, 152), S( 155, 140), S( 180, 134), S( 305, 136), S( 138, 158), S( 157, 134),
    S( 104, 130), S( 108, 166), S( 140, 158), S( 171, 162), S( 139, 174), S( 155, 146), S( 107, 152), S(  89, 153),
    S(  93, 110), S( 115, 158), S( 110, 164), S( 142, 172), S( 154, 164), S( 113, 161), S( 111, 135), S( 116,  76),
    S( 105, 117), S( 113, 149), S( 108, 174), S( 117, 158), S( 108, 187), S( 117, 144), S( 121,  88), S( 126, 118),
    S( 119,  76), S( 104, 102), S( 115, 104), S(  93, 129), S( 103, 143), S(  74, 140), S( 140, 106), S( 106,  56),
    S(  36, 101), S(  58,  82), S(  78,  77), S(  64, 121), S(  35, 125), S(  70,  92), S( -73, 164), S(  91,  61),
    };

    tables[WKnight] = {
    S(-187,  72), S( -71,  78), S( -76, 137), S(  89,  55), S( 202,  45), S( -92, 132), S(  79,  20), S(-167,  32),
    S( -20,  84), S(   9,  83), S(  77,  91), S( 126,  99), S( 124,  60), S( 184,  35), S( -41,  74), S( -12,  61),
    S(   3,  78), S(  57, 105), S(  78, 120), S( 120, 117), S( 191,  80), S( 260,  56), S( 118,  65), S(  36,  60),
    S(  43,  80), S(  39, 114), S(  76, 127), S(  97, 134), S(  64, 131), S( 140,  89), S(  47, 117), S(  89,  57),
    S(  17,  76), S(  20,  89), S(  48, 132), S(  28, 133), S(  43, 133), S(  47, 124), S(  42, 109), S(  18,  61),
    S( -10,  50), S(   7,  78), S(  23,  95), S(  43, 108), S(  30, 119), S(  28,  98), S(  43,  77), S( -12,  61),
    S( -34,  42), S( -26,  38), S( -18,  78), S(  15,  78), S(  20,  79), S(   1, 102), S(  20,  78), S(   8,  11),
    S( -80,   7), S(  -2, -41), S( -50,  46), S( -23,  53), S( -23,  62), S( -18,  36), S(  -1,   0), S( -34, -38),
    };

    tables[WRook] = {
    S( 168, 270), S( 191, 277), S( 224, 267), S( 220, 275), S( 277, 247), S( 196, 279), S( 205, 279), S( 240, 240),
    S( 157, 292), S( 164, 290), S( 210, 276), S( 221, 283), S( 226, 268), S( 359, 203), S( 362, 212), S( 317, 226),
    S( 137, 278), S( 184, 261), S( 206, 257), S( 203, 257), S( 287, 221), S( 283, 230), S( 293, 223), S( 269, 216),
    S( 106, 274), S( 132, 282), S( 160, 267), S( 176, 264), S( 182, 246), S( 225, 234), S( 143, 269), S( 128, 243),
    S( 110, 257), S( 133, 252), S( 131, 258), S( 144, 259), S( 145, 240), S( 124, 252), S( 153, 227), S( 137, 219),
    S(  71, 242), S( 101, 241), S( 116, 242), S( 120, 226), S( 130, 235), S( 125, 237), S( 176, 217), S( 132, 195),
    S(  82, 220), S(  92, 233), S( 122, 236), S(  99, 239), S( 101, 235), S( 135, 225), S( 127, 203), S(  45, 210),
    S( 113, 229), S( 124, 226), S( 130, 247), S( 131, 249), S( 138, 238), S( 126, 228), S(  96, 230), S( 110, 158),
    };

    tables[WPawn] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( 111, 190), S(  61, 215), S( 119, 151), S( 115,  79), S(  82, 132), S( -44, 147), S( -52, 159), S(-207, 206),
    S(  -1, 147), S(  25, 140), S(  31, 117), S(  56,  57), S(  80,  49), S( 130,  37), S(  87,  78), S(   4,  95),
    S(  15,  92), S(  32,  78), S(  17,  61), S(  42,  23), S(  48,  32), S(  44,  47), S(  55,  56), S(  36,  56),
    S(   3,  75), S(  30,  68), S(  13,  43), S(  25,  45), S(  28,  34), S(  34,  51), S(  64,  43), S(  39,  37),
    S(   3,  76), S(  24,  68), S(   2,  53), S(   7,  55), S(  33,  71), S(  11,  65), S( 103,  40), S(  59,  28),
    S(  -8,  79), S(  13,  70), S( -11,  64), S( -15,  22), S(  12,  67), S(  63,  71), S( 133,  35), S(  47,  20),
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