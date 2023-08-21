#include "board.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(BoardTest, getPieceValidSquare) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(0, A);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    ASSERT_EQ(getPieceResult, BRook) << getPieceResult;
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
    bool setPieceOutcome = defaultBoard.setPiece(square, BPawn);
    EXPECT_EQ(setPieceOutcome, true);
    EXPECT_EQ(defaultBoard.getPiece(square), BPawn);
}

TEST(BoardTest, setPieceInvalidSquare1) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(-1, D);
    bool setPieceOutcome = defaultBoard.setPiece(square, WPawn);
    EXPECT_EQ(setPieceOutcome, false);
}

TEST(BoardTest, setPieceInvalidSquare2) {
    Board defaultBoard = Board();
    BoardSquare square = BoardSquare(0, 8);
    bool setPieceOutcome = defaultBoard.setPiece(square, WPawn);
    EXPECT_EQ(setPieceOutcome, false);
}

TEST(BoardTest, boardSquareStrConstructor) {
    BoardSquare square = BoardSquare("a8");
    EXPECT_EQ(square.rank, 0);
    EXPECT_EQ(square.file, 0);
}

TEST(BoardTest, boardSquareStrConstructorNeg) {
    BoardSquare square = BoardSquare("-");
    EXPECT_EQ(square, BoardSquare());
}

TEST(BoardTest, boardMoveStrConstructor) {
    BoardMove move = BoardMove("e2e4");
    EXPECT_EQ(move.pos1.rank, 6);
    EXPECT_EQ(move.pos1.file, 4);
    EXPECT_EQ(move.pos2.rank, 4);
    EXPECT_EQ(move.pos2.file, 4);
}

TEST(BoardTest, boardMoveStrConstructor2) {
    BoardMove move = BoardMove("a7a8q", true);
    EXPECT_EQ(move.pos1.rank, 1);
    EXPECT_EQ(move.pos1.file, 0);
    EXPECT_EQ(move.pos2.rank, 0);
    EXPECT_EQ(move.pos2.file, 0);
    EXPECT_EQ(move.promotionPiece, WQueen);
}

TEST(BoardTest, fenConstructorDefault) {
    Board fenBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board defaultBoard = Board();
    EXPECT_EQ(fenBoard.board, defaultBoard.board);
    EXPECT_EQ(fenBoard.isWhiteTurn, defaultBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, defaultBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, defaultBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, defaultBoard.fiftyMoveRule);
}

TEST(BoardTest, fenConstructorEnPassantSquare) {
    Board fenBoard = Board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    Board moveBoard = Board();
    moveBoard = Board(moveBoard, BoardMove("e2e4"));

    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, moveBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, moveBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, moveBoard.fiftyMoveRule);
}

TEST(BoardTest, fenConstructorEnPassantCastle) {
    // ruy lopez
    Board fenBoard = Board("r1bqkb1r/pppp1ppp/2n2n2/1B2p3/4P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4");
    Board moveBoard = Board();
    moveBoard = Board(moveBoard, BoardMove("e2e4"));
    moveBoard = Board(moveBoard, BoardMove("e7e5"));
    moveBoard = Board(moveBoard, BoardMove("g1f3"));
    moveBoard = Board(moveBoard, BoardMove("b8c6"));
    moveBoard = Board(moveBoard, BoardMove("f1b5"));
    moveBoard = Board(moveBoard, BoardMove("g8f6"));
    moveBoard = Board(moveBoard, BoardMove("e1g1"));

    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, moveBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, moveBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, moveBoard.fiftyMoveRule);
}

TEST(CastleRightsBitTest, defaultBoard) {
    Board board = Board();
    EXPECT_EQ(castleRightsBit(BoardSquare(0, G)) && board.castlingRights, true);
    EXPECT_EQ(castleRightsBit(BoardSquare(0, C)) && board.castlingRights, true);
    EXPECT_EQ(castleRightsBit(BoardSquare(7, G)) && board.castlingRights, true);
    EXPECT_EQ(castleRightsBit(BoardSquare(7, C)) && board.castlingRights, true);    
    EXPECT_EQ(castleRightsBit(BoardSquare(4, E)) && board.castlingRights, false);    
}
