#pragma once

#include "move.hpp"
#include "types.hpp"

namespace Eval {

constexpr int totalPhase = 24;

class Info {
    public:
        Info() = default;
        int getRawEval(const PieceSets& pieceSets) const;
        void addPiece(Square square, pieceTypes piece);
        void removePiece(Square square, pieceTypes piece);

    private:
        template<bool ISOPENING>
        int evalPawns(const PieceSets& pieceSets) const;
        template<bool ISOPENING, bool ISWHITE>
        int evalPassedPawns(const PieceSets& pieceSets) const;
        int mopUpScore(const PieceSets& pieceSets, int score) const;

        int opScore{};
        int egScore{};
        int phase{};
};

template<bool IS_OPENING>
int getPlacementScore(Square square, pieceTypes currPiece);
int getPiecePhase(pieceTypes piece);

constexpr std::array<int, NUM_RANKS> passedPawnOp = {0, 65, 38, 15, -1, -15, 2, 0, };
constexpr std::array<int, NUM_RANKS> passedPawnEg = {0, 193, 109, 65, 33, 6, -6, 0, };

constexpr std::array<int, NUM_PIECES> pieceValsOp = {0, 854, 307, 359, 389, 65, };
constexpr std::array<int, NUM_PIECES> pieceValsEg = {0, 802, 279, 284, 475, 93, };

constexpr auto tablesOp = [] {
    std::array<std::array<int, BOARD_SIZE>, 6> tables{};

    tables[WKing] = {
    38, 399, -40, -133, 465, 874, -141, -72,
    999, 148, 124, 417, 83, -59, 286, -354,
    354, 145, -167, -32, 56, 153, -45, -176,
    -208, -248, 30, -209, -82, 39, -175, -278,
    -364, -204, -190, -119, -175, -223, -177, -278,
    -168, -124, -136, -230, -299, -215, -72, -108,
    -14, -210, -67, -142, -137, -125, -18, 41,
    -9, -7, -26, -137, 6, -110, 51, 50,
    };

    tables[WQueen] = {
    340, 415, 402, 393, 435, 406, 499, 485,
    367, 365, 392, 363, 334, 508, 374, 490,
    391, 383, 368, 418, 444, 485, 483, 413,
    380, 380, 389, 393, 398, 405, 379, 397,
    393, 383, 389, 381, 375, 386, 397, 367,
    378, 396, 382, 380, 385, 389, 402, 399,
    361, 381, 402, 401, 401, 408, 421, 381,
    383, 347, 383, 393, 400, 315, 256, 354,
    };

    tables[WBishop] = {
    96, 64, -8, 21, -29, -119, -94, 94,
    118, 126, 125, 84, 95, 178, 82, 96,
    102, 124, 146, 152, 183, 294, 148, 163,
    118, 112, 137, 176, 142, 153, 108, 87,
    99, 114, 113, 143, 158, 117, 106, 113,
    106, 111, 108, 123, 112, 123, 123, 117,
    122, 109, 119, 98, 107, 59, 145, 89,
    30, 42, 82, 74, 29, 72, -123, 122,
    };

    tables[WKnight] = {
    -203, -85, -111, 50, 255, -102, 39, -187,
    -18, 7, 67, 104, 127, 184, -45, -18,
    29, 69, 70, 121, 183, 244, 121, 44,
    35, 36, 74, 99, 68, 142, 47, 93,
    24, 24, 49, 30, 46, 51, 53, 23,
    -5, 16, 28, 56, 34, 32, 50, -14,
    -29, -24, -11, 18, 23, -5, 27, 9,
    -101, 2, -43, -30, -20, -1, 1, -30,
    };

    tables[WRook] = {
    160, 193, 221, 217, 261, 228, 232, 281,
    148, 156, 199, 201, 216, 347, 371, 313,
    129, 176, 192, 193, 277, 278, 290, 266,
    88, 131, 155, 165, 176, 197, 141, 141,
    90, 142, 118, 134, 146, 120, 162, 130,
    72, 101, 113, 115, 123, 126, 182, 126,
    63, 78, 117, 98, 93, 132, 116, 38,
    107, 117, 126, 126, 135, 125, 80, 106,
    };

    tables[WPawn] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    90, 68, 110, 120, 56, -51, -62, -145,
    -13, 28, 36, 63, 85, 128, 86, 32,
    6, 28, 12, 40, 47, 43, 47, 22,
    1, 26, 9, 23, 26, 26, 58, 38,
    -3, 24, 1, 8, 31, 7, 96, 56,
    -16, 12, -11, -17, 12, 58, 129, 42,
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
    -268, -305, 26, 59, -221, -590, 95, -49,
    -575, -59, -115, -176, -55, 63, -203, 218,
    -163, -51, 113, 44, 28, 1, 115, 124,
    61, 115, 2, 117, 98, 37, 100, 113,
    128, 86, 99, 76, 105, 125, 97, 101,
    87, 81, 79, 115, 133, 118, 57, 49,
    7, 103, 70, 83, 82, 64, 30, -16,
    -69, -9, 5, 19, -81, 5, -36, -71,
    };

    tables[WQueen] = {
    538, 538, 574, 582, 602, 637, 521, 548,
    515, 552, 561, 622, 687, 586, 684, 543,
    465, 502, 606, 551, 620, 636, 635, 617,
    483, 529, 565, 596, 615, 659, 665, 558,
    457, 524, 519, 609, 590, 561, 542, 565,
    461, 438, 524, 513, 510, 531, 532, 428,
    469, 463, 431, 448, 444, 397, 360, 365,
    447, 463, 404, 418, 388, 414, 336, 393,
    };

    tables[WBishop] = {
    148, 133, 190, 197, 178, 253, 162, 72,
    111, 141, 167, 168, 191, 129, 190, 137,
    122, 173, 162, 155, 143, 168, 164, 147,
    140, 174, 173, 174, 194, 153, 167, 175,
    109, 173, 178, 188, 178, 171, 153, 82,
    126, 146, 194, 165, 205, 158, 91, 138,
    74, 105, 97, 141, 153, 155, 116, 39,
    101, 110, 93, 122, 150, 95, 169, 26,
    };

    tables[WKnight] = {
    112, 103, 153, 68, 9, 135, 80, 11,
    86, 80, 103, 112, 51, 48, 81, 64,
    69, 108, 128, 119, 90, 71, 73, 65,
    86, 121, 139, 143, 137, 92, 117, 66,
    76, 80, 144, 135, 138, 133, 98, 62,
    42, 74, 98, 102, 127, 101, 84, 50,
    23, 33, 81, 89, 81, 114, 96, 18,
    7, -37, 39, 63, 60, 35, -24, -34,
    };

    tables[WRook] = {
    281, 293, 279, 283, 271, 277, 269, 233,
    312, 310, 295, 310, 287, 228, 224, 250,
    299, 284, 283, 277, 242, 251, 244, 232,
    297, 297, 289, 281, 266, 264, 291, 249,
    284, 263, 283, 284, 253, 262, 230, 239,
    254, 260, 257, 249, 263, 260, 224, 218,
    249, 247, 255, 261, 259, 248, 230, 221,
    249, 247, 268, 270, 259, 250, 257, 178,
    };

    tables[WPawn] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    201, 235, 170, 80, 149, 139, 174, 182,
    169, 154, 132, 68, 60, 46, 100, 87,
    99, 83, 65, 24, 33, 41, 61, 63,
    78, 74, 46, 41, 38, 54, 45, 37,
    76, 68, 53, 52, 70, 66, 46, 28,
    78, 73, 64, 16, 64, 73, 35, 17,
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