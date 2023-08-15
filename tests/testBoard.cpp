#include "board.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(BoardTest, getPieceValidSquare) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(0, A);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    ASSERT_EQ(getPieceResult, BRookUnmoved) << getPieceResult;
}

TEST(BoardTest, getPieceValidSquare2) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(1, H);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    EXPECT_EQ(getPieceResult, BPawn);
}

TEST(BoardTest, getPieceValidSquare3) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(6, D);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    EXPECT_EQ(getPieceResult, WPawn);
}

TEST(BoardTest, getPieceInvalidSquare1) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(-1, A);
    ASSERT_EQ(defaultBoard.getPiece(square), nullPiece);
}

TEST(BoardTest, getPieceInvalidSquare2) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(8, A);
    ASSERT_EQ(defaultBoard.getPiece(square), nullPiece);
}

TEST(BoardTest, getPieceInvalidSquare3) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(0, -1);
    ASSERT_EQ(defaultBoard.getPiece(square), nullPiece);
}

TEST(BoardTest, getPieceInvalidSquare4) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(0, 8);
    ASSERT_EQ(defaultBoard.getPiece(square), nullPiece);
}

TEST(BoardTest, setPieceValidSquare1) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(1, A);
    bool setPieceOutcome = defaultBoard.setPiece(square, BRook);
    EXPECT_EQ(setPieceOutcome, true);
    EXPECT_EQ(defaultBoard.getPiece(square), BRook);
}

TEST(BoardTest, setPieceValidSquare2) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(4, D);
    bool setPieceOutcome = defaultBoard.setPiece(square, BPawnJumped);
    EXPECT_EQ(setPieceOutcome, true);
    EXPECT_EQ(defaultBoard.getPiece(square), BPawnJumped);
}

TEST(BoardTest, setPieceInvalidSquare1) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(-1, D);
    bool setPieceOutcome = defaultBoard.setPiece(square, WPawnJumped);
    EXPECT_EQ(setPieceOutcome, false);
}

TEST(BoardTest, setPieceInvalidSquare2) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(0, 8);
    bool setPieceOutcome = defaultBoard.setPiece(square, WPawnJumped);
    EXPECT_EQ(setPieceOutcome, false);
}
