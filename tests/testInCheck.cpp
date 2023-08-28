#include "inCheck.hpp"

#include <gtest/gtest.h>
#include <array>

TEST(InCheckTest, isFriendlyPieceTrue1) {
    Board board;
    BoardSquare whiteSquare = BoardSquare(7, H);
    bool result = isFriendlyPiece(board, whiteSquare);
    ASSERT_EQ(result, true);
}

TEST(InCheckTest, isFriendlyPieceTrue2) {
    Board board;
    board.isWhiteTurn = false;
    BoardSquare blackSquare = BoardSquare(1, H);
    bool result = isFriendlyPiece(board, blackSquare);
    ASSERT_EQ(result, true);
}

TEST(InCheckTest, isFriendlyPieceFalse) {
    Board board;
    BoardSquare blackSquare = BoardSquare(1, H);
    bool result = isFriendlyPiece(board, blackSquare);
    ASSERT_EQ(result, false);
}

TEST(InCheckTest, getPieceInDirectionStraight) {
    Board board;
    BoardSquare originSquare = BoardSquare(1, D); // black pawn
    pieceTypes down = getPieceInDirection(board, originSquare, 1, 0);
    pieceTypes up = getPieceInDirection(board, originSquare, -1, 0);
    pieceTypes right = getPieceInDirection(board, originSquare, 0, 1);
    pieceTypes left = getPieceInDirection(board, originSquare, 0, -1);
    EXPECT_EQ(down, WPawn);
    EXPECT_EQ(up, BQueen);
    EXPECT_EQ(right, BPawn);
    EXPECT_EQ(left, BPawn);
}

TEST(InCheckTest, getPieceInDirectionDiag) {
    Board board;
    BoardSquare originSquare = BoardSquare(1, E); // black pawn
    pieceTypes topRight = getPieceInDirection(board, originSquare, -1, 1);
    pieceTypes topLeft = getPieceInDirection(board, originSquare, -1, -1);
    pieceTypes bottomLeft = getPieceInDirection(board, originSquare, 1, -1);
    pieceTypes bottomRight = getPieceInDirection(board, originSquare, 1, 1);
    EXPECT_EQ(topRight, BBishop);
    EXPECT_EQ(topLeft, BQueen);
    EXPECT_EQ(bottomLeft, nullPiece);
    EXPECT_EQ(bottomRight, nullPiece);
}

TEST(InCheckTest, checkDiagAttackersTrue) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WQueen    , EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkDiagAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkDiagAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WRook     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WBishop   , BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkDiagAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, false);
}


TEST(InCheckTest, checkStraightAttackersTrue) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkStraightAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}


TEST(InCheckTest, checkStraightAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WRook     , EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BRook     , BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkStraightAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, checkKnightAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKnightAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKnightAttackersTrue3) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKnightAttackersTrue4) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKnightAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BKnight   , WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, checkPawnAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkPawnAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkPawnAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkPawnAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkPawnAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkPawnAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, checkKingAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKingAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKingAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKingAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKingAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKingAttackers(board, originSquare);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, inCheckDefaultBoard) {
    Board board;
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, inCheckTrue1) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, inCheckTrue2) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , BRook     , EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, BoardMoveConstructorPawnJump) {
    Board defaultBoard;
    BoardSquare pos1 = BoardSquare(6, E);
    BoardSquare pos2 = BoardSquare(4, E);
    BoardSquare enPassantSquare = BoardSquare(5, E);
    Board board(defaultBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.pawnJumpedSquare, enPassantSquare);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST(InCheckTest, BoardMoveConstructorKingCastle) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook,
        BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawn, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WPawn, WPawn, WPawn, WPawn, EmptyPiece, WPawn, WPawn, WPawn,
        WRook, WKnight, WBishop, WQueen, WKing, EmptyPiece, EmptyPiece, WRook,
    };
    Board originBoard(boardArr, true);
    BoardSquare pos1 = BoardSquare(7, E);
    BoardSquare pos2 = BoardSquare(7, G);
    Board board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, H), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, F), WRook);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST(InCheckTest, BoardMoveConstructorQueenCastle) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook,
        BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawn, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WPawn, WPawn, WPawn, WPawn, EmptyPiece, WPawn, WPawn, WPawn,
        WRook, EmptyPiece, EmptyPiece, EmptyPiece, WKing, WBishop, WKnight, WRook,
    };
    Board originBoard(boardArr, true);
    BoardSquare pos1 = BoardSquare(7, E);
    BoardSquare pos2 = BoardSquare(7, C);
    Board board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, A), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, D), WRook);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST(InCheckTest, BoardMoveConstructorMovedKing) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook,
        BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawn, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WPawn, WPawn, WPawn, WPawn, EmptyPiece, WPawn, WPawn, WPawn,
        WRook, EmptyPiece, EmptyPiece, EmptyPiece, WKing, WBishop, WKnight, WRook,
    };
    Board originBoard(boardArr, true);
    BoardSquare pos1 = BoardSquare(7, E);
    BoardSquare pos2 = BoardSquare(6, D);
    Board board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, A), WRook);
    EXPECT_EQ(board.getPiece(7, D), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST(InCheckTest, BoardMoveConstructorKingToCastleSquare) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     , EmptyPiece, WRook     ,
    };
    Board originBoard(boardArr, true);
    originBoard.castlingRights = B_Castle;
    BoardSquare pos1 = BoardSquare(7, F);
    BoardSquare pos2 = BoardSquare(7, G);
    Board board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.getPiece(7, H), WRook);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST(InCheckTest, BoardMoveConstructorEnPassant) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board originBoard(boardArr, true);
    BoardSquare jumpedPawn = BoardSquare(3, E);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = originBoard.pawnJumpedSquare = BoardSquare(2, E);
    Board board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(jumpedPawn), EmptyPiece);
    EXPECT_EQ(board.pawnJumpedSquare, BoardSquare());
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST(InCheckTest, BoardMoveConstructorNotEnPassant) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board originBoard(boardArr, true);
    BoardSquare jumpedPawn = originBoard.pawnJumpedSquare = BoardSquare(3, E);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(2, D);
    Board board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(jumpedPawn), BPawn);
    EXPECT_EQ(board.pawnJumpedSquare, BoardSquare());
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST(InCheckTest, BoardMoveConstructorPromote) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WPawn     , WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board originBoard(boardArr, true);
    BoardSquare pos1 = BoardSquare(1, A);
    BoardSquare pos2 = BoardSquare(0, A);
    Board board(originBoard, pos1, pos2, WQueen);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WQueen);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST(InCheckTest, BoardMoveConstructorPawnCapture) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board originBoard(boardArr, true);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(2, E);
    Board board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(3, E), BPawn);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST(InCheckTest, BoardMoveConstructorRegularCapture) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board originBoard(boardArr, true);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(1, F);
    Board board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WBishop);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST(InCheckTest, BoardMoveConstructorRookPin) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board originBoard(boardArr, true);
    BoardSquare pos1 = BoardSquare(1, A);
    BoardSquare pos2 = BoardSquare(1, B);
    Board board(originBoard, pos1, pos2);
    ASSERT_EQ(board.isIllegalPos, true);
}

TEST(InCheckTest, BoardMoveConstructorBishopPin) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board originBoard(boardArr, true);
    BoardSquare pos1 = BoardSquare(6, B);
    BoardSquare pos2 = BoardSquare(5, A);
    Board board(originBoard, pos1, pos2);
    ASSERT_EQ(board.isIllegalPos, true);
}

TEST(InCheckTest, BoardMoveConstructorCastleRightsRook) {
    Board originBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");
    Board board(originBoard, BoardMove("h1g1", originBoard.isWhiteTurn));
    EXPECT_EQ(originBoard.castlingRights, All_Castle);
    EXPECT_EQ(board.castlingRights, B_Castle | W_OOO);
}

TEST(InCheckTest, BoardMoveConstructorCastleRightsRook2) {
    Board originBoard("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    Board board(originBoard, BoardMove("h8g8", originBoard.isWhiteTurn));
    EXPECT_EQ(originBoard.castlingRights, All_Castle);
    EXPECT_EQ(board.castlingRights, W_Castle | B_OOO);
}

TEST(InCheckTest, BoardMoveConstructorCastleRightsRook3) {
    Board originBoard("7r/1k4P1/1n6/B7/P4P1p/7P/4NK2/1R5R b - - 0 44");
    Board board(originBoard, BoardMove("h8g8", originBoard.isWhiteTurn));
    EXPECT_EQ(originBoard.castlingRights, noCastle);
    EXPECT_EQ(board.castlingRights, noCastle);
}

