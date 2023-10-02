#include "bitboard.hpp"
#include "types.hpp"

#include <gtest/gtest.h>
#include <cstdint>
#include <bitset>
#include <array>



TEST(BitboardTest, leadingBitEmpty) {
    uint64_t bitboard = 0ull;
    ASSERT_EQ(leadingBit(bitboard), -1);
}

TEST(BitboardTest, leadingBitOne) {
    uint64_t bitboard = 1ull;
    ASSERT_EQ(leadingBit(bitboard), 0);
}

TEST(BitboardTest, leadingBitTrailingOne) {
    uint64_t bitboard = 0x8000000000000000ull;
    ASSERT_EQ(leadingBit(bitboard), 63);
}

TEST(BitboardTest, leadingBitMiddleOne) {
    uint64_t bitboard = 0x0000000000001000ull;
    ASSERT_EQ(leadingBit(bitboard), 12);
}

TEST(BitboardTest, leadingBitMultOnes) {
    uint64_t bitboard = 0x0000001000100000ull;
    ASSERT_EQ(leadingBit(bitboard), 20);
}

TEST(BitboardTest, trailingBitEmpty) {
    uint64_t bitboard = 0ull;
    ASSERT_EQ(trailingBit(bitboard), -1);
}

TEST(BitboardTest, trailingBitOne) {
    uint64_t bitboard = 0x8000000000000000ull;
    ASSERT_EQ(trailingBit(bitboard), 0);
}

TEST(BitboardTest, trailingBitForwardOne) {
    uint64_t bitboard = 0x0000000000000001ull;
    ASSERT_EQ(trailingBit(bitboard), 63);
}

TEST(BitboardTest, trailingBitMiddleOne) {
    uint64_t bitboard = 0x0008000000000000ull;
    ASSERT_EQ(trailingBit(bitboard), 12);
}

TEST(BitboardTest, trailingBitMultOnes) {
    uint64_t bitboard = 0x0000080010000000ull;
    ASSERT_EQ(trailingBit(bitboard), 20);
}

TEST(BitboardTest, popLeadingBit1) {
    uint64_t bitboard = 0x0000001000100000ull;
    ASSERT_EQ(popLeadingBit(bitboard), 20);
    ASSERT_EQ(bitboard, 0x0000001000000000ull);
}

TEST(BitboardTest, popTrailingBit1) {
    uint64_t bitboard = 0x0000080010000000ull;
    ASSERT_EQ(popTrailingBit(bitboard), 20);
    ASSERT_EQ(bitboard, 0x0000000010000000ull);
}

TEST(BitboardTest, FlipVerticalEmpty) {
    uint64_t bitboard = 0x0000000000000000ull;
    ASSERT_EQ(flipVertical(bitboard), 0ull);
}

TEST(BitboardTest, FlipVerticalOne) {
    uint64_t bitboard = 0x0000000000000001ull;
    uint64_t expected = 0x0100000000000000ull;
    ASSERT_EQ(flipVertical(bitboard), expected);
}

TEST(BitboardTest, FlipVerticalMul) {
    uint64_t bitboard = 0x0000000000008421ull;
    uint64_t expected = 0x2184000000000000ull;
    ASSERT_EQ(flipVertical(bitboard), expected);
}

TEST(BitboardTest, getFileMaskTest) {
    EXPECT_EQ(getFileMask(0), FILE_A);
    EXPECT_EQ(getFileMask(1), FILE_B);
    EXPECT_EQ(getFileMask(2), FILE_C);
    EXPECT_EQ(getFileMask(3), FILE_D);
    EXPECT_EQ(getFileMask(4), FILE_E);
    EXPECT_EQ(getFileMask(5), FILE_F);
    EXPECT_EQ(getFileMask(6), FILE_G);
    EXPECT_EQ(getFileMask(7), FILE_H);
    EXPECT_EQ(getFileMask(8), FILE_A);
}

TEST(BitboardTest, getRankMaskTest) {
    EXPECT_EQ(getRankMask(0), RANK_8);
    EXPECT_EQ(getRankMask(8), RANK_7);
    EXPECT_EQ(getRankMask(16), RANK_6);
    EXPECT_EQ(getRankMask(24), RANK_5);
    EXPECT_EQ(getRankMask(32), RANK_4);
    EXPECT_EQ(getRankMask(40), RANK_3);
    EXPECT_EQ(getRankMask(48), RANK_2);
    EXPECT_EQ(getRankMask(56), RANK_1);
    EXPECT_EQ(getRankMask(5), RANK_8);
}

TEST(BitboardTest, getDiagMaskTest) {
    EXPECT_EQ(getDiagMask(0 ), DIAG_0);
    EXPECT_EQ(getDiagMask(8 ), DIAG_1);
    EXPECT_EQ(getDiagMask(16), DIAG_2);
    EXPECT_EQ(getDiagMask(24), DIAG_3);
    EXPECT_EQ(getDiagMask(32), DIAG_4);
    EXPECT_EQ(getDiagMask(40), DIAG_5);
    EXPECT_EQ(getDiagMask(48), DIAG_6);
    EXPECT_EQ(getDiagMask(56), DIAG_7);
    EXPECT_EQ(getDiagMask(57), DIAG_8);
    EXPECT_EQ(getDiagMask(58), DIAG_9);
    EXPECT_EQ(getDiagMask(59), DIAG_10);
    EXPECT_EQ(getDiagMask(60), DIAG_11);
    EXPECT_EQ(getDiagMask(61), DIAG_12);
    EXPECT_EQ(getDiagMask(62), DIAG_13);
    EXPECT_EQ(getDiagMask(63), DIAG_14);
    EXPECT_EQ(getDiagMask(1 ), DIAG_1);
}

TEST(BitboardTest, diagAttackersFalse1) {
    uint64_t enemies = 0ull;
    uint64_t allies = 1ull << 30;
    int square = 30;

    EXPECT_EQ(diagAttackers(square, allies | enemies, enemies), false);
}

TEST(BitboardTest, diagAttackersFalse2) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WQueen    , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 2, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(diagAttackers(square, allies | enemies, enemies), false);
}

TEST(BitboardTest, diagAttackersFalse3) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BBishop   , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BBishop   , EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 2, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(diagAttackers(square, allies | enemies, enemies), false);
}

TEST(BitboardTest, diagAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WQueen    , EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(diagAttackers(square, allies | enemies, enemies), true);
}

TEST(BitboardTest, diagAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 2, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(diagAttackers(square, allies | enemies, enemies), true);
}

TEST(BitboardTest, diagAttackersTrue3) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WQueen    , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 2, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(diagAttackers(square, allies | enemies, enemies), true);
}

TEST(BitboardTest, straightAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BRook     , BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WRook     , EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WRook     , EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(straightAttackers(square, allies | enemies, enemies), false);
}

TEST(BitboardTest, straightAttackersFalse2) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BRook     , BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WRook     , EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WRook     , EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(straightAttackers(square, allies | enemies, enemies), false);
}

TEST(BitboardTest, straightAttackersFalse3) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BRook     , BKing     , BRook     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WRook     , EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece, WRook     , EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WRook     , EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(straightAttackers(square, allies | enemies, enemies), false);
}

TEST(BitboardTest, straightAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(straightAttackers(square, allies | enemies, enemies), true);
}

TEST(BitboardTest, straightAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> alliesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;
    ASSERT_EQ(alliesBoard.at(square), BKing);

    uint64_t allies = arrayToBitboardNotEmpty(alliesBoard);
    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(straightAttackers(square, allies | enemies, enemies), true);
}

TEST(BitboardTest, knightSquares1) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);
    uint64_t knightBitboard = knightSquares(enemies);
    EXPECT_EQ(knightBitboard, 0x0000000000020400ull);
}

TEST(BitboardTest, knightSquares2) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);
    uint64_t knightBitboard = knightSquares(enemies);
    EXPECT_EQ(knightBitboard, 0x0000050800080500ull);
}

TEST(BitboardTest, knightSquares3) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);
    uint64_t knightBitboard = knightSquares(enemies);
    EXPECT_EQ(knightBitboard, 0x0508000805000000ull);
}

TEST(BitboardTest, knightSquares4) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);
    uint64_t knightBitboard = knightSquares(enemies);
    EXPECT_EQ(knightBitboard, 0x1100110A00000000ull);
}

TEST(BitboardTest, knightSquares5) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece,
    };

    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);
    uint64_t knightBitboard = knightSquares(enemies);
    EXPECT_EQ(knightBitboard, 0x22442A1400000000ull);
}

TEST(BitboardTest, pawnAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    uint64_t pawnBitboard = arrayToBitboardNotEmpty(enemiesBoard);
    uint64_t attackedSquares = pawnAttackSquares(pawnBitboard, true);
    printBitboard(attackedSquares);
    EXPECT_EQ(attackedSquares, 0x0000000a00000000ull);
}

TEST(BitboardTest, pawnAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    uint64_t pawnBitboard = arrayToBitboardNotEmpty(enemiesBoard);
    uint64_t attackedSquares = pawnAttackSquares(pawnBitboard, true);
    EXPECT_EQ(attackedSquares, 0x0000002800000000ull);
}

TEST(BitboardTest, pawnAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    uint64_t pawnBitboard = arrayToBitboardNotEmpty(enemiesBoard);
    uint64_t attackedSquares = pawnAttackSquares(pawnBitboard, false);
    EXPECT_EQ(attackedSquares, 0x0000002A00000000ull);
}

TEST(BitboardTest, kingAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;

    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(kingAttackers(square, enemies), true);
}

TEST(BitboardTest, kingAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;

    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(kingAttackers(square, enemies), true);
}

TEST(BitboardTest, kingAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> enemiesBoard = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };

    int rank = 4, file = 3;
    int square = rank * 8 + file;

    uint64_t enemies = arrayToBitboardNotEmpty(enemiesBoard);

    EXPECT_EQ(kingAttackers(square, enemies), false);
}