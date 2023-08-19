#include "inCheck.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(InCheckTest, isFriendlyPieceTrue1) {
    Board board = Board();
    BoardSquare whiteSquare = BoardSquare(7, H);
    bool result = isFriendlyPiece(board, whiteSquare);
    ASSERT_EQ(result, true);
}

TEST(InCheckTest, isFriendlyPieceTrue2) {
    Board board = Board();
    board.isWhiteTurn = false;
    BoardSquare blackSquare = BoardSquare(1, H);
    bool result = isFriendlyPiece(board, blackSquare);
    ASSERT_EQ(result, true);
}

TEST(InCheckTest, isFriendlyPieceFalse) {
    Board board = Board();
    BoardSquare blackSquare = BoardSquare(1, H);
    bool result = isFriendlyPiece(board, blackSquare);
    ASSERT_EQ(result, false);
}

TEST(InCheckTest, getPieceInDirectionStraight) {
    Board board = Board();
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
    Board board = Board();
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
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WQueen, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkDiagAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkDiagAttackersFalse) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WRook     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WBishop   , BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkDiagAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, false);
}


TEST(InCheckTest, checkStraightAttackersTrue) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece},
        {WRook     , EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkStraightAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}


TEST(InCheckTest, checkStraightAttackersFalse) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WRook     , EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, BRook     , BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkStraightAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, checkKnightAttackersTrue1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKnightAttackersTrue2) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKnightAttackersTrue3) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKnightAttackersTrue4) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKnightAttackersFalse) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, BKnight   , WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKnightAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, checkPawnAttackersTrue1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkPawnAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkPawnAttackersTrue2) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawnJumped, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkPawnAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkPawnAttackersFalse) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, WPawnJumped, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkPawnAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, checkKingAttackersTrue1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKingAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKingAttackersTrue2) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WKingUnmoved, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKingAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, checkKingAttackersFalse) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkKingAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, inCheckDefaultBoard) {
    Board board = Board();
    bool isAttacked = currKingInAttackAfterMove(board);
    ASSERT_EQ(isAttacked, false);
}

TEST(InCheckTest, inCheckTrue1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WRook     , EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    bool isAttacked = currKingInAttackAfterMove(board);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, inCheckTrue2) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WRook     , BRook     , EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector, false);
    bool isAttacked = currKingInAttackAfterMove(board);
    ASSERT_EQ(isAttacked, true);
}

TEST(InCheckTest, BoardMoveConstructorPawnJump) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {BRookUnmoved, BKnight, BBishop, BQueen, BKingUnmoved, BBishop, BKnight, BRookUnmoved},
        {BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawnJumped, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WPawn, WPawn, WPawn, WPawn, EmptyPiece, WPawn, WPawn, WPawn},
        {WRookUnmoved, WKnight, WBishop, WQueen, WKingUnmoved, WBishop, WKnight, WRookUnmoved}
    };
    Board defaultBoard = Board();
    BoardSquare pos1 = BoardSquare(6, E);
    BoardSquare pos2 = BoardSquare(4, E);
    Board board = Board(defaultBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos2), WPawnJumped);
    EXPECT_EQ(board.pawnJumped, true);
    EXPECT_EQ(board.pawnJumpedSquare, pos2);
    EXPECT_EQ(board.movesSincePawnMovedOrCapture, 0);
}

TEST(InCheckTest, BoardMoveConstructorKingCastle) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {BRookUnmoved, BKnight, BBishop, BQueen, BKingUnmoved, BBishop, BKnight, BRookUnmoved},
        {BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawnJumped, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WPawn, WPawn, WPawn, WPawn, EmptyPiece, WPawn, WPawn, WPawn},
        {WRookUnmoved, WKnight, WBishop, WQueen, WKingUnmoved, EmptyPiece, EmptyPiece, WRookUnmoved}
    };
    Board originBoard = Board(boardVector, true);
    BoardSquare pos1 = BoardSquare(7, E);
    BoardSquare pos2 = BoardSquare(7, H);
    Board board = Board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, F), WRook);
    EXPECT_EQ(board.getPiece(7, G), WKing);
    EXPECT_EQ(board.pawnJumped, false);
    EXPECT_EQ(board.movesSincePawnMovedOrCapture, 1);
}

TEST(InCheckTest, BoardMoveConstructorQueenCastle) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {BRookUnmoved, BKnight, BBishop, BQueen, BKingUnmoved, BBishop, BKnight, BRookUnmoved},
        {BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawnJumped, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WPawn, WPawn, WPawn, WPawn, EmptyPiece, WPawn, WPawn, WPawn},
        {WRookUnmoved, EmptyPiece, EmptyPiece, EmptyPiece, WKingUnmoved, WBishop, WKnight, WRookUnmoved}
    };
    Board originBoard = Board(boardVector, true);
    BoardSquare pos1 = BoardSquare(7, E);
    BoardSquare pos2 = BoardSquare(7, A);
    Board board = Board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, D), WRook);
    EXPECT_EQ(board.getPiece(7, C), WKing);
    EXPECT_EQ(board.pawnJumped, false);
    EXPECT_EQ(board.movesSincePawnMovedOrCapture, 1);
}

TEST(InCheckTest, BoardMoveConstructorEnPassant) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , BPawnJumped, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board originBoard = Board(boardVector, true);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(2, E);
    Board board = Board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(3, E), EmptyPiece);
    EXPECT_EQ(board.pawnJumped, false);
    EXPECT_EQ(board.movesSincePawnMovedOrCapture, 0);
}

TEST(InCheckTest, BoardMoveConstructorNotEnPassant) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , BPawnJumped, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board originBoard = Board(boardVector, true);
    originBoard.pawnJumped = true;
    BoardSquare jumpedPawn =  originBoard.pawnJumpedSquare = BoardSquare(3, E);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(2, D);
    Board board = Board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(jumpedPawn), BPawn);
    EXPECT_EQ(board.pawnJumped, false);
    EXPECT_EQ(board.movesSincePawnMovedOrCapture, 0);
}

TEST(InCheckTest, BoardMoveConstructorPromote) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WPawn     , WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board originBoard = Board(boardVector, true);
    BoardSquare pos1 = BoardSquare(1, A);
    BoardSquare pos2 = BoardSquare(0, A);
    Board board = Board(originBoard, pos1, pos2, WQueen);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WQueen);
    EXPECT_EQ(board.movesSincePawnMovedOrCapture, 0);
}

TEST(InCheckTest, BoardMoveConstructorPawnCapture) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board originBoard = Board(boardVector, true);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(2, E);
    Board board = Board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(3, E), BPawn);
    EXPECT_EQ(board.pawnJumped, false);
    EXPECT_EQ(board.movesSincePawnMovedOrCapture, 0);
}

TEST(InCheckTest, BoardMoveConstructorRegularCapture) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board originBoard = Board(boardVector, true);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(1, F);
    Board board = Board(originBoard, pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WBishop);
    EXPECT_EQ(board.pawnJumped, false);
    EXPECT_EQ(board.movesSincePawnMovedOrCapture, 0);
}

TEST(InCheckTest, BoardMoveConstructorRookPin) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board originBoard = Board(boardVector, true);
    BoardSquare pos1 = BoardSquare(1, A);
    BoardSquare pos2 = BoardSquare(1, B);
    Board board = Board(originBoard, pos1, pos2);
    ASSERT_EQ(board.isIllegalPos, true);
}

TEST(InCheckTest, BoardMoveConstructorBishopPin) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, BBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board originBoard = Board(boardVector, true);
    BoardSquare pos1 = BoardSquare(6, B);
    BoardSquare pos2 = BoardSquare(5, A);
    Board board = Board(originBoard, pos1, pos2);
    ASSERT_EQ(board.isIllegalPos, true);
}
