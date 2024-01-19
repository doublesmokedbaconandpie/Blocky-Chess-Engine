#pragma once

#include "move.hpp"
#include "bitboard.hpp"
#include "types.hpp"

namespace Eval {

constexpr int totalPhase = 24;
// pawn hash has a prime number size to prevent collisions
constexpr int PAWN_HASH_SIZE = 1024;

struct PawnHashEntry {
    int opScore{};
    int egScore{};
    uint64_t key{};
};

// eval scores
struct S {
    S(int op, int eg) : opScore(op), egScore(eg) {};
    int opScore{};
    int egScore{};
};

class Info {
    public:
        Info() = default;
        int getRawEval(const PieceSets& pieceSets);
        void addPiece(Square square, pieceTypes piece);
        void removePiece(Square square, pieceTypes piece);

    private:
        const PawnHashEntry& getPawnInfo(const PieceSets& pieceSets);
        template<bool ISOPENING>
        int evalPassedPawns(const PieceSets& pieceSets, bool isWhiteTurn) const;
        int mopUpScore(const PieceSets& pieceSets, int score) const;

        int opScore{};
        int egScore{};
        int phase{};
        std::array<PawnHashEntry, PAWN_HASH_SIZE> pawnHashTable{};
        uint64_t pawnKey{};
};

template<bool IS_OPENING>
int getPlacementScore(Square square, pieceTypes currPiece);
int getPiecePhase(pieceTypes piece);

constexpr std::array<int, NUM_RANKS> passedPawnOp = {0, 69, 55, 20, -1, -21, 3, 0, };
constexpr std::array<int, NUM_RANKS> passedPawnEg = {0, 186, 107, 63, 34, 11, -4, 0, };

constexpr std::array<int, NUM_PIECES> pieceValsOp = {0, 896, 333, 384, 417, 69, };
constexpr std::array<int, NUM_PIECES> pieceValsEg = {0, 785, 271, 274, 470, 90, };

constexpr auto tablesOp = [] {
    std::array<std::array<int, BOARD_SIZE>, 6> tables{};

    tables[WKing] = {
    32, 376, -11, -125, 238, 954, -153, 16,
    895, 57, 74, 205, 59, -9, 262, 293,
    189, 26, -155, -82, 13, 148, -20, -92,
    -142, -232, 33, -161, -68, 180, -111, -219,
    -364, -227, -193, -173, -184, -184, -153, -281,
    -123, -163, -147, -241, -268, -216, -84, -109,
    -26, -158, -64, -135, -134, -119, -14, 37,
    -8, -10, -31, -142, 3, -113, 48, 44,
    };

    tables[WQueen] = {
    353, 420, 428, 408, 426, 407, 507, 506,
    348, 351, 392, 353, 342, 517, 384, 504,
    386, 375, 366, 408, 429, 497, 485, 403,
    368, 377, 376, 373, 384, 394, 372, 392,
    384, 375, 381, 374, 366, 381, 388, 359,
    383, 382, 377, 370, 372, 382, 397, 382,
    355, 376, 392, 389, 395, 411, 403, 359,
    368, 356, 372, 386, 388, 313, 325, 345,
    };

    tables[WBishop] = {
    100, 32, -27, 41, -53, -109, -27, 133,
    106, 121, 128, 94, 105, 160, 88, 76,
    107, 113, 148, 155, 180, 305, 138, 157,
    104, 108, 140, 171, 139, 155, 107, 89,
    93, 115, 110, 142, 154, 113, 111, 116,
    105, 113, 108, 117, 108, 117, 121, 126,
    119, 104, 115, 93, 103, 74, 140, 106,
    36, 58, 78, 64, 35, 70, -73, 91,
    };

    tables[WKnight] = {
    -187, -71, -76, 89, 202, -92, 79, -167,
    -20, 9, 77, 126, 124, 184, -41, -12,
    3, 57, 78, 120, 191, 260, 118, 36,
    43, 39, 76, 97, 64, 140, 47, 89,
    17, 20, 48, 28, 43, 47, 42, 18,
    -10, 7, 23, 43, 30, 28, 43, -12,
    -34, -26, -18, 15, 20, 1, 20, 8,
    -80, -2, -50, -23, -23, -18, -1, -34,
    };

    tables[WRook] = {
    168, 191, 224, 220, 277, 196, 205, 240,
    157, 164, 210, 221, 226, 359, 362, 317,
    137, 184, 206, 203, 287, 283, 293, 269,
    106, 132, 160, 176, 182, 225, 143, 128,
    110, 133, 131, 144, 145, 124, 153, 137,
    71, 101, 116, 120, 130, 125, 176, 132,
    82, 92, 122, 99, 101, 135, 127, 45,
    113, 124, 130, 131, 138, 126, 96, 110,
    };

    tables[WPawn] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    111, 61, 119, 115, 82, -44, -52, -207,
    -1, 25, 31, 56, 80, 130, 87, 4,
    15, 32, 17, 42, 48, 44, 55, 36,
    3, 30, 13, 25, 28, 34, 64, 39,
    3, 24, 2, 7, 33, 11, 103, 59,
    -8, 13, -11, -15, 12, 63, 133, 47,
    0, 0, 0, 0, 0, 0, 0, 0,
    };

    for (int piece = 0; piece < NUM_PIECES; ++piece) {
        for (auto& sqr: tables[piece]) {
            sqr += pieceValsOp[piece];
        }
    }

    return tables;
}();

// Endgame tables

constexpr auto tablesEg = [] {
    std::array<std::array<int, BOARD_SIZE>, 6> tables{};

    tables[WKing] = {
    -248, -280, 46, 117, -65, -587, 114, -6,
    -539, 6, -51, -64, -65, 35, -178, -83,
    -77, 11, 104, 62, 22, -3, 66, 72,
    52, 122, 4, 88, 75, -15, 82, 93,
    125, 101, 97, 97, 103, 110, 83, 100,
    72, 91, 80, 113, 125, 113, 58, 44,
    -2, 77, 58, 75, 72, 59, 27, -21,
    -73, -18, 9, 19, -83, -0, -38, -72,
    };

    tables[WQueen] = {
    514, 530, 535, 568, 586, 613, 502, 506,
    522, 547, 542, 606, 653, 536, 651, 501,
    440, 490, 581, 546, 617, 601, 588, 597,
    484, 498, 552, 590, 608, 641, 638, 531,
    434, 493, 520, 588, 576, 535, 518, 546,
    410, 445, 498, 495, 496, 516, 492, 428,
    442, 441, 418, 440, 425, 358, 349, 342,
    456, 420, 390, 397, 375, 401, 239, 404,
    };

    tables[WBishop] = {
    116, 145, 181, 179, 191, 214, 156, 108,
    105, 133, 152, 157, 163, 130, 160, 134,
    109, 162, 152, 140, 134, 136, 158, 134,
    130, 166, 158, 162, 174, 146, 152, 153,
    110, 158, 164, 172, 164, 161, 135, 76,
    117, 149, 174, 158, 187, 144, 88, 118,
    76, 102, 104, 129, 143, 140, 106, 56,
    101, 82, 77, 121, 125, 92, 164, 61,
    };

    tables[WKnight] = {
    72, 78, 137, 55, 45, 132, 20, 32,
    84, 83, 91, 99, 60, 35, 74, 61,
    78, 105, 120, 117, 80, 56, 65, 60,
    80, 114, 127, 134, 131, 89, 117, 57,
    76, 89, 132, 133, 133, 124, 109, 61,
    50, 78, 95, 108, 119, 98, 77, 61,
    42, 38, 78, 78, 79, 102, 78, 11,
    7, -41, 46, 53, 62, 36, -0, -38,
    };

    tables[WRook] = {
    270, 277, 267, 275, 247, 279, 279, 240,
    292, 290, 276, 283, 268, 203, 212, 226,
    278, 261, 257, 257, 221, 230, 223, 216,
    274, 282, 267, 264, 246, 234, 269, 243,
    257, 252, 258, 259, 240, 252, 227, 219,
    242, 241, 242, 226, 235, 237, 217, 195,
    220, 233, 236, 239, 235, 225, 203, 210,
    229, 226, 247, 249, 238, 228, 230, 158,
    };

    tables[WPawn] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    190, 215, 151, 79, 132, 147, 159, 206,
    147, 140, 117, 57, 49, 37, 78, 95,
    92, 78, 61, 23, 32, 47, 56, 56,
    75, 68, 43, 45, 34, 51, 43, 37,
    76, 68, 53, 55, 71, 65, 40, 28,
    79, 70, 64, 22, 67, 71, 35, 20,
    0, 0, 0, 0, 0, 0, 0, 0,
    };

    for (int piece = 0; piece < NUM_PIECES; ++piece) {
        for (auto& sqr: tables[piece]) {
            sqr += pieceValsEg[piece];
        }
    }

    return tables;
}();

} // namespace eval