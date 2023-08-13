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
    bool isWhiteTurn = false;
    Board board = Board(boardVector, isWhiteTurn);
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
        {EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece},
    };
    bool isWhiteTurn = false;
    Board board = Board(boardVector, isWhiteTurn);
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
    bool isWhiteTurn = false;
    Board board = Board(boardVector, isWhiteTurn);
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
    bool isWhiteTurn = false;
    Board board = Board(boardVector, isWhiteTurn);
    BoardSquare originSquare = BoardSquare(4, D);
    bool isAttacked = checkStraightAttackers(board, originSquare, BKing);
    ASSERT_EQ(isAttacked, false);
}


