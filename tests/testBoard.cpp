#include "board.hpp"
#include "attacks.hpp"
#include "zobrist.hpp"

#include <gtest/gtest.h>
#include <string>

class BoardTest : public testing::Test {
    public:
        static void SetUpTestSuite() {
            Attacks::init();
        }
};

TEST(SquareTest, SquareStrConstructor) {
    Square square = toSquare("a8");
    EXPECT_EQ(square, 0);
}

TEST(SquareTest, SquareStrConstructorNeg) {
    Square square = toSquare("-");
    EXPECT_EQ(square, NULLSQUARE);
}

TEST(MoveTest, MoveStrConstructor) {
    BoardMove move = BoardMove("e2e4", true);
    EXPECT_EQ(move.sqr1(), 52);
    EXPECT_EQ(move.sqr2(), 36);
}

TEST(MoveTest, MoveStrConstructor2) {
    BoardMove move = BoardMove("a7a8q", true);
    EXPECT_EQ(move.sqr1(), 8);
    EXPECT_EQ(move.sqr2(), 0);
    EXPECT_EQ(move.getPromotePiece(), WQueen);
}

TEST_F(BoardTest, getPieceValidSquare) {
    Board defaultBoard;
    Square square = toSquare("a8");
    ASSERT_EQ(defaultBoard.getPiece(square), BRook);
}

TEST_F(BoardTest, getPieceValidSquare2) {
    Board defaultBoard;
    Square square = toSquare("a7");
    ASSERT_EQ(defaultBoard.getPiece(square), BPawn);
}

TEST_F(BoardTest, getPieceValidSquare3) {
    Board defaultBoard;
    Square square = toSquare("d2");
    ASSERT_EQ(defaultBoard.getPiece(square), WPawn);
}

TEST_F(BoardTest, fenConstructorDefault) {
    Board fenBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board defaultBoard;

    EXPECT_EQ(fenBoard.pieceSets, defaultBoard.pieceSets);
    EXPECT_EQ(fenBoard.board, defaultBoard.board);
    EXPECT_EQ(fenBoard.getZobristKey(), defaultBoard.getZobristKey());
    EXPECT_NE(fenBoard.getZobristKey(), 0);
    EXPECT_EQ(fenBoard.getIsWhiteTurn(), defaultBoard.getIsWhiteTurn());
    EXPECT_EQ(fenBoard.getCastlingRights(), defaultBoard.getCastlingRights());
    EXPECT_EQ(fenBoard.getEnPassSquare(), defaultBoard.getEnPassSquare());
    EXPECT_EQ(fenBoard.getFiftyMoveRule(), defaultBoard.getFiftyMoveRule());
}

TEST_F(BoardTest, fenConstructorEnPassantSquare) {
    Board fenBoard("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    Board moveBoard;
    moveBoard.makeMove(BoardMove("e2e4", moveBoard.getIsWhiteTurn()));

    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.getZobristKey(), moveBoard.getZobristKey());
    EXPECT_NE(fenBoard.getZobristKey(), 0);
    EXPECT_EQ(fenBoard.getIsWhiteTurn(), moveBoard.getIsWhiteTurn());
    EXPECT_EQ(fenBoard.getCastlingRights(), moveBoard.getCastlingRights());
    EXPECT_EQ(fenBoard.getEnPassSquare(), moveBoard.getEnPassSquare());
    EXPECT_EQ(fenBoard.getFiftyMoveRule(), moveBoard.getFiftyMoveRule());
}

TEST_F(BoardTest, fenConstructorEnPassantCastle) {
    // ruy lopez
    Board fenBoard("r1bqkb1r/pppp1ppp/2n2n2/1B2p3/4P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4");
    Board moveBoard;
    moveBoard.makeMove(BoardMove("e2e4", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("e7e5", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("g1f3", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("b8c6", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("f1b5", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("g8f6", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("e1g1", moveBoard.getIsWhiteTurn()));

    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.getZobristKey(), moveBoard.getZobristKey());
    EXPECT_NE(fenBoard.getZobristKey(), 0);
    EXPECT_EQ(fenBoard.getIsWhiteTurn(), moveBoard.getIsWhiteTurn());
    EXPECT_EQ(fenBoard.getCastlingRights(), moveBoard.getCastlingRights());
    EXPECT_EQ(fenBoard.getEnPassSquare(), moveBoard.getEnPassSquare());
    EXPECT_EQ(fenBoard.getFiftyMoveRule(), moveBoard.getFiftyMoveRule());
}

TEST_F(BoardTest, toFenDefault) {
    std::string expectedFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Board board(expectedFen);
    ASSERT_EQ(board.toFen(), expectedFen);
}

TEST_F(BoardTest, toFenMoves) {
    std::string expectedFen = "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 1";
    Board board(expectedFen);
    ASSERT_EQ(board.toFen(), expectedFen);
}

TEST_F(BoardTest, toFenCastling) {
    std::string expectedFen = "r1b1kbnr/ppppqppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 1";
    Board board(expectedFen);
    ASSERT_EQ(board.toFen(), expectedFen);
}

TEST(CastleRightsBitTest, defaultBoard) {
    Board board;
    EXPECT_EQ(castleRightsBit(toSquare("g8"), board.getIsWhiteTurn()) && board.getCastlingRights(), false); // is not black's turn
    EXPECT_EQ(castleRightsBit(toSquare("c8"), board.getIsWhiteTurn()) && board.getCastlingRights(), false); // is not black's turn
    EXPECT_EQ(castleRightsBit(toSquare("g1"), board.getIsWhiteTurn()) && board.getCastlingRights(), true);
    EXPECT_EQ(castleRightsBit(toSquare("c1"), board.getIsWhiteTurn()) && board.getCastlingRights(), true);
    EXPECT_EQ(castleRightsBit(toSquare("e3"), board.getIsWhiteTurn()) && board.getCastlingRights(), false);
}

TEST_F(BoardTest, checkDiagAttackersTrue) {
    Board board("8/8/8/8/3k4/3r4/8/6QK b - - 0 1");
    bool isAttacked = currKingInAttack(board.pieceSets, board.getIsWhiteTurn());
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkDiagAttackersFalse) {
    Board board("7K/8/5B2/4p3/2Bk4/8/8/6p1 b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkStraightAttackersTrue) {
    Board board("7K/8/8/4p3/R2k4/8/8/6p1 b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkStraightAttackersFalse) {
    Board board("7K/8/8/2R5/2rk4/8/8/6p1 b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkKnightAttackersTrue1) {
    Board board("8/8/2N5/8/3k4/8/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkKnightAttackersTrue2) {
    Board board("8/8/8/1N6/3k4/8/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkKnightAttackersTrue3) {
    Board board("8/8/8/8/3k4/1N6/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkKnightAttackersTrue4) {
    Board board("8/8/8/8/3k4/8/2N5/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkKnightAttackersFalse) {
    Board board("8/8/8/8/3k4/8/2nN4/4N2K b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkPawnAttackersTrue1) {
    Board board("8/8/8/8/3k4/2P5/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkPawnAttackersTrue2) {
    Board board("8/8/8/8/3k4/4P3/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkPawnAttackersFalse) {
    Board board("8/8/8/2P1P3/3k4/8/8/7K b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkKingAttackersTrue1) {
    Board board("8/8/8/2K5/3k4/8/8/8 b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkKingAttackersTrue2) {
    Board board("8/8/8/2K5/3k4/8/8/8 b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, checkKingAttackersFalse) {
    Board board("8/8/8/8/3k4/8/8/7K b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, inCheckTrue1) {
    Board board("8/8/8/8/R2k4/8/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, inCheckTrue2) {
    Board board("8/8/1B6/8/Rr1k4/8/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.getIsWhiteTurn()));
}

TEST_F(BoardTest, MakeMovePawnJump) {
    Board board;
    Square pos1 = toSquare("e2");
    Square pos2 = toSquare("e4");
    Square enPassantSquare = toSquare("e3");
    board.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(board.getIsWhiteTurn(), false);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getEnPassSquare(), enPassantSquare);
    EXPECT_EQ(board.getFiftyMoveRule(), 0);
}

TEST_F(BoardTest, MakeMoveKingCastle) {
    Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQK2R w KQkq - 0 1");
    Square pos1 = toSquare("e1");
    Square pos2 = toSquare("g1");
    Square rookStart = toSquare("h1");
    Square rookEnd = toSquare("f1");
    board.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(board.getIsWhiteTurn(), false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(rookStart), EmptyPiece);
    EXPECT_EQ(board.getPiece(rookEnd), WRook);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.getFiftyMoveRule(), 1);
    EXPECT_EQ(board.getCastlingRights(), B_Castle);
}

TEST_F(BoardTest, MakeMoveQueenCastle) {
    Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/R3KBNR w KQkq - 0 1");
    Square pos1 = toSquare("e1");
    Square pos2 = toSquare("c1");
    Square rookStart = toSquare("a1");
    Square rookEnd = toSquare("d1");
    board.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(board.getIsWhiteTurn(), false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(rookStart), EmptyPiece);
    EXPECT_EQ(board.getPiece(rookEnd), WRook);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.getFiftyMoveRule(), 1);
    EXPECT_EQ(board.getCastlingRights(), B_Castle);
}

TEST_F(BoardTest, MakeMoveMovedKing) {
    Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/R3KBNR w KQkq - 0 1");
    Square pos1 = toSquare("e1");
    Square pos2 = toSquare("e2");
    board.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(board.getIsWhiteTurn(), false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.getFiftyMoveRule(), 1);
    EXPECT_EQ(board.getCastlingRights(), B_Castle);
}

TEST_F(BoardTest, MakeMoveKingToCastleSquare) {
    Board board("k7/8/8/8/8/8/8/5K1R w kq - 0 1");
    Square pos1 = toSquare("f1");
    Square pos2 = toSquare("g1");
    board.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(board.getIsWhiteTurn(), false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.getPiece(toSquare("h1")), WRook);
    EXPECT_EQ(board.getFiftyMoveRule(), 1);
    EXPECT_EQ(board.getCastlingRights(), B_Castle);
}

TEST_F(BoardTest, MakeMoveEnPassant) {
    Board fenBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    Board moveBoard("rnbqkb1r/ppp1pppp/3P1n2/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3");
    Square pos1 = toSquare("e5");
    Square pos2 = toSquare("d6");
    Square enemyPawn = toSquare("d5");
    EXPECT_EQ(fenBoard.getPiece(enemyPawn), BPawn);
    fenBoard.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(fenBoard.getIsWhiteTurn(), moveBoard.getIsWhiteTurn());
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(fenBoard.getPiece(pos2), WPawn);
    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.getEnPassSquare(), NULLSQUARE);
    EXPECT_EQ(fenBoard.getZobristKey(), moveBoard.getZobristKey());
    EXPECT_EQ(fenBoard.getFiftyMoveRule(), 0);   
}

TEST_F(BoardTest, MakeMoveNotEnPassant) {
    Board fenBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    Board moveBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 3");
    Square pos1 = toSquare("g1");
    Square pos2 = toSquare("f3");
    Square enemyPawn = toSquare("d5");
    EXPECT_EQ(fenBoard.getPiece(enemyPawn), BPawn);
    fenBoard.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(fenBoard.getIsWhiteTurn(), moveBoard.getIsWhiteTurn());
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.getEnPassSquare(), NULLSQUARE);
    EXPECT_EQ(fenBoard.getZobristKey(), moveBoard.getZobristKey());
    EXPECT_EQ(fenBoard.getFiftyMoveRule(), 1);   
}

TEST_F(BoardTest, MakeMovePromote) {
    Board board("8/PK6/8/8/8/8/8/7k w KQkq - 0 1");
    Square pos1 = toSquare("a7");
    Square pos2 = toSquare("a8");
    board.makeMove(BoardMove(pos1, pos2, WQueen));

    EXPECT_EQ(board.getIsWhiteTurn(), false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WQueen);
    EXPECT_EQ(board.getFiftyMoveRule(), 0);
}

TEST_F(BoardTest, MakeMovePawnCapture) {
    Board board("8/8/4p3/3Pp3/8/K7/8/k7 w KQkq - 0 1");
    Square pos1 = toSquare("d5");
    Square pos2 = toSquare("e6");
    board.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(board.getIsWhiteTurn(), false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(toSquare("e5")), BPawn);
    EXPECT_EQ(board.getFiftyMoveRule(), 0);
}

TEST_F(BoardTest, LegalMoveRegularCapture) {
    Board board("8/5p2/8/3Bp3/8/K7/8/k7 w KQkq - 0 1");
    Square pos1 = toSquare("d5");
    Square pos2 = toSquare("f7");
    board.makeMove(BoardMove(pos1, pos2));

    EXPECT_EQ(board.getIsWhiteTurn(), false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WBishop);
    EXPECT_EQ(board.getFiftyMoveRule(), 0);
}

TEST_F(BoardTest, LegalMoveRookPin) {
    Board board("r6k/R7/8/8/8/8/8/K7 w KQkq - 0 1");
    BoardMove move("a7b7", board.getIsWhiteTurn());
    ASSERT_FALSE(board.isLegalMove(move));
}

TEST_F(BoardTest, LegalMoveBishopPin) {
    Board board("8/8/8/8/3b4/7k/1B6/K7 w KQkq - 0 1");
    BoardMove move("b2a3", board.getIsWhiteTurn());
    ASSERT_FALSE(board.isLegalMove(move));
}

TEST_F(BoardTest, LegalMoveEnPassant) {
    Board board("2RQ4/R6p/6b1/1k5p/Ppp5/8/6P1/7K b - a3 0 42");
    BoardMove move("b4a3", board.getIsWhiteTurn());
    ASSERT_TRUE(board.isLegalMove(move));
}

TEST_F(BoardTest, LegalMoveEnPassantPin) {
    Board board("6k1/8/7p/3K1Ppq/4n3/8/6p1/8 w - g6 0 1");
    BoardMove move("f5g6", board.getIsWhiteTurn());
    ASSERT_FALSE(board.isLegalMove(move));
}

TEST_F(BoardTest, MakeMoveCastleRightsRook) {
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");
    board.makeMove(BoardMove("h1g1", board.getIsWhiteTurn()));
    EXPECT_EQ(board.getCastlingRights(), B_Castle | W_OOO);
}

TEST_F(BoardTest, MakeMoveCastleRightsRook2) {
    Board board("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    board.makeMove(BoardMove("h8g8", board.getIsWhiteTurn()));
    EXPECT_EQ(board.getCastlingRights(), W_Castle | B_OOO);
}

TEST_F(BoardTest, MakeMoveCastleRightsRook3) {
    Board board("7r/1k4P1/1n6/B7/P4P1p/7P/4NK2/1R5R b - - 0 44");
    board.makeMove(BoardMove("h8g8", board.getIsWhiteTurn()));
    EXPECT_EQ(board.getCastlingRights(), noCastle);
}

TEST_F(BoardTest, undoMove) {
    // ruy lopez
    Board defaultBoard, moveBoard;
    moveBoard.makeMove(BoardMove("e2e4", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("e7e5", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("g1f3", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("b8c6", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("f1b5", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("g8f6", moveBoard.getIsWhiteTurn()));
    moveBoard.makeMove(BoardMove("e1g1", moveBoard.getIsWhiteTurn()));
    for (int i = 0; i < 7; i++) {
        moveBoard.undoMove();
    }

    EXPECT_EQ(defaultBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(defaultBoard.board, moveBoard.board);
    EXPECT_EQ(defaultBoard.getZobristKey(), moveBoard.getZobristKey());
    EXPECT_NE(defaultBoard.getZobristKey(), 0);
    EXPECT_EQ(defaultBoard.getIsWhiteTurn(), moveBoard.getIsWhiteTurn());
    EXPECT_EQ(defaultBoard.getCastlingRights(), moveBoard.getCastlingRights());
    EXPECT_EQ(defaultBoard.getEnPassSquare(), moveBoard.getEnPassSquare());
    EXPECT_EQ(defaultBoard.getFiftyMoveRule(), moveBoard.getFiftyMoveRule());
}

TEST_F(BoardTest, moveIsCaptureTest) {
    Board b1;
    BoardMove m1 = BoardMove("e2e4", b1.getIsWhiteTurn());
    bool b_1 = b1.moveIsCapture(m1); //F
    b1.makeMove(m1);
    BoardMove m2 = BoardMove("d7d5", b1.getIsWhiteTurn());
    bool b_2 = b1.moveIsCapture(m2); //F
    b1.makeMove(m2);
    BoardMove m3 = BoardMove("e4e5", b1.getIsWhiteTurn());
    bool b_3 = b1.moveIsCapture(m3); //F
    b1.makeMove(m3);
    BoardMove m4 = BoardMove("d5d4", b1.getIsWhiteTurn());
    bool b_4 = b1.moveIsCapture(m4); //F
    b1.makeMove(m4);
    BoardMove m5 = BoardMove("h2h3", b1.getIsWhiteTurn());
    bool b_5 = b1.moveIsCapture(m5); //F
    b1.makeMove(m5);
    BoardMove m6 = BoardMove("f7f5", b1.getIsWhiteTurn());
    bool b_6 = b1.moveIsCapture(m6); //F
    b1.makeMove(m6);
    BoardMove m7 = BoardMove("e5f6", b1.getIsWhiteTurn());
    bool b_7 = b1.moveIsCapture(m7); //T
    b1.makeMove(m7);
    BoardMove m8 = BoardMove("g7f6", b1.getIsWhiteTurn());
    bool b_8 = b1.moveIsCapture(m8); //T
    b1.makeMove(m8);
    BoardMove m9 = BoardMove("c2c4", b1.getIsWhiteTurn());
    bool b_9 = b1.moveIsCapture(m9); //F
    b1.makeMove(m9);
    BoardMove m10 = BoardMove("d4c3", b1.getIsWhiteTurn());
    bool b_10 = b1.moveIsCapture(m10); //T
    b1.makeMove(m10);
    BoardMove m11 = BoardMove("d2d4", b1.getIsWhiteTurn());
    bool b_11 = b1.moveIsCapture(m11); //F
    b1.makeMove(m11);
    BoardMove m12 = BoardMove("c3b2", b1.getIsWhiteTurn());
    bool b_12 = b1.moveIsCapture(m12); //T
    b1.makeMove(m12);
    BoardMove m13 = BoardMove("d4d5", b1.getIsWhiteTurn());
    bool b_13 = b1.moveIsCapture(m13); //F
    b1.makeMove(m13);
    BoardMove m14 = BoardMove("b2a1r", b1.getIsWhiteTurn());
    bool b_14 = b1.moveIsCapture(m14); //T
    b1.makeMove(m14);
    

    EXPECT_EQ(b_1, false);
    EXPECT_EQ(b_2, false);
    EXPECT_EQ(b_3, false);
    EXPECT_EQ(b_4, false);
    EXPECT_EQ(b_5, false);
    EXPECT_EQ(b_6, false);
    EXPECT_EQ(b_7, true);
    EXPECT_EQ(b_8, true);
    EXPECT_EQ(b_9, false);
    EXPECT_EQ(b_10, true);
    EXPECT_EQ(b_11, false);
    EXPECT_EQ(b_12, true);
    EXPECT_EQ(b_13, false);
    EXPECT_EQ(b_14, true);
}