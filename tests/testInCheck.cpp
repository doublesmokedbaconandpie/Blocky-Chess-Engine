#include "inCheck.hpp"

#include <gtest/gtest.h>

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