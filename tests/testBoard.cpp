#include "board.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(BoardTest, getPieceValidSquare) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(0, A);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    ASSERT_EQ(getPieceResult, BRook) << getPieceResult;
}

TEST(BoardTest, getPieceValidSquare2) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(1, H);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    EXPECT_EQ(getPieceResult, BPawn);
}

TEST(BoardTest, getPieceValidSquare3) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(6, D);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    EXPECT_EQ(getPieceResult, WPawn);
}

TEST(BoardTest, getPieceInvalidSquare1) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(-1, A);
    ASSERT_EQ(defaultBoard.getPiece(square), nullPiece);
}

TEST(BoardTest, getPieceInvalidSquare2) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(8, A);
    ASSERT_EQ(defaultBoard.getPiece(square), nullPiece);
}

TEST(BoardTest, getPieceInvalidSquare3) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(0, -1);
    ASSERT_EQ(defaultBoard.getPiece(square), nullPiece);
}

TEST(BoardTest, getPieceInvalidSquare4) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(0, 8);
    ASSERT_EQ(defaultBoard.getPiece(square), nullPiece);
}

TEST(BoardTest, setPieceValidSquare1) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(1, A);
    bool setPieceOutcome = defaultBoard.setPiece(square, BRook);
    EXPECT_EQ(setPieceOutcome, true);
    EXPECT_EQ(defaultBoard.getPiece(square), BRook);
}

TEST(BoardTest, setPieceValidSquare2) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(4, D);
    bool setPieceOutcome = defaultBoard.setPiece(square, BPawn);
    EXPECT_EQ(setPieceOutcome, true);
    EXPECT_EQ(defaultBoard.getPiece(square), BPawn);
}

TEST(BoardTest, setPieceInvalidSquare1) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(-1, D);
    bool setPieceOutcome = defaultBoard.setPiece(square, WPawn);
    EXPECT_EQ(setPieceOutcome, false);
}

TEST(BoardTest, setPieceInvalidSquare2) {
    Board defaultBoard;
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
    BoardMove move = BoardMove("e2e4", true);
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
    Board fenBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board defaultBoard;
    EXPECT_EQ(fenBoard.board, defaultBoard.board);
    EXPECT_EQ(fenBoard.isWhiteTurn, defaultBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, defaultBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, defaultBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, defaultBoard.fiftyMoveRule);
}

TEST(BoardTest, fenConstructorEnPassantSquare) {
    Board fenBoard("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    Board moveBoard;
    moveBoard.makeMove(BoardMove("e2e4", moveBoard.isWhiteTurn));

    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, moveBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, moveBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, moveBoard.fiftyMoveRule);
}

TEST(BoardTest, fenConstructorEnPassantCastle) {
    // ruy lopez
    Board fenBoard("r1bqkb1r/pppp1ppp/2n2n2/1B2p3/4P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4");
    Board moveBoard;
    moveBoard.makeMove(BoardMove("e2e4", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("e7e5", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("g1f3", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("b8c6", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("f1b5", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("g8f6", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("e1g1", moveBoard.isWhiteTurn));

    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, moveBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, moveBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, moveBoard.fiftyMoveRule);
}

TEST(CastleRightsBitTest, defaultBoard) {
    Board board;
    EXPECT_EQ(castleRightsBit(BoardSquare(0, G)) && board.castlingRights, true);
    EXPECT_EQ(castleRightsBit(BoardSquare(0, C)) && board.castlingRights, true);
    EXPECT_EQ(castleRightsBit(BoardSquare(7, G)) && board.castlingRights, true);
    EXPECT_EQ(castleRightsBit(BoardSquare(7, C)) && board.castlingRights, true);    
    EXPECT_EQ(castleRightsBit(BoardSquare(4, E)) && board.castlingRights, false);    
}

TEST(MaterialDifferenceTest, defaultBoard) {
    Board board;
    Board fenDefaultBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(board.materialDifference, 0);
    EXPECT_EQ(fenDefaultBoard.materialDifference, 0);
}

TEST(MaterialDifferenceTest, fenBoards) {
    
    Board fenBoard1("r1b1kbnr/pppBpppp/3q4/8/8/2N5/PPPP1PPP/R1BQK1NR b KQkq - 0 1");
    Board fenBoard2("r1bqkbnr/pp1ppppp/8/8/3nP3/8/PPP2PPP/RNBQKB1R w KQkq - 0 1");
    Board fenBoard3("rnbqkbnr/8/8/8/PP4PP/8/2PPPP2/3QK3 w kq - 0 1");

    EXPECT_EQ(fenBoard1.materialDifference, 3);
    EXPECT_EQ(fenBoard2.materialDifference, -3);
    EXPECT_EQ(fenBoard3.materialDifference, -14);
}

TEST(MaterialDifferenceTest, captureTests) {
    Board b1;
    b1.makeMove(BoardMove("e2e4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d7d5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("e4d5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("c8h3", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d5d6", b1.isWhiteTurn)); //+1
    int eval1 = b1.materialDifference;
    b1.makeMove(BoardMove("h3g2", b1.isWhiteTurn)); //+-0
    int eval2 = b1.materialDifference;
    b1.makeMove(BoardMove("d6c7", b1.isWhiteTurn)); //+1
    int eval3 = b1.materialDifference;
    b1.makeMove(BoardMove("g2h1", b1.isWhiteTurn)); //-4
    int eval4 = b1.materialDifference;
    b1.makeMove(BoardMove("c7b8q", b1.isWhiteTurn)); //+7
    int eval5 = b1.materialDifference;

    EXPECT_EQ(eval1, 1);
    EXPECT_EQ(eval2, 0);
    EXPECT_EQ(eval3, 1);
    EXPECT_EQ(eval4, -4);
    EXPECT_EQ(eval5, 7);
}

TEST(MaterialDifferenceTest, enPassantTests) {
    Board b1;
    b1.makeMove(BoardMove("e2e4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d7d5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("e4e5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d5d4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("h2h3", b1.isWhiteTurn));
    b1.makeMove(BoardMove("f7f5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("e5f6", b1.isWhiteTurn)); //+1
    int eval1 = b1.materialDifference;
    b1.makeMove(BoardMove("g7f6", b1.isWhiteTurn)); //+-0
    b1.makeMove(BoardMove("c2c4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d4c3", b1.isWhiteTurn)); //-1
    int eval2 = b1.materialDifference;
    b1.makeMove(BoardMove("d2d4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("c3b2", b1.isWhiteTurn)); //-2
    b1.makeMove(BoardMove("d4d5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("b2a1r", b1.isWhiteTurn)); //-11
    int eval3 = b1.materialDifference;
    
    EXPECT_EQ(eval1, 1);
    EXPECT_EQ(eval2, -1);
    EXPECT_EQ(eval3, -11);
}
