#include "board.hpp"
#include "attacks.hpp"
#include "zobrist.hpp"

#include <gtest/gtest.h>
#include <string>

class BoardTest : public testing::Test {
    public:
        static void SetUpTestSuite() {
            Zobrist::init();
            Attacks::init();
        }
};

TEST(SquareTest, SquareStrConstructor) {
    BoardSquare square = BoardSquare("a8");
    EXPECT_EQ(square.rank, 0);
    EXPECT_EQ(square.file, 0);
}

TEST(SquareTest, SquareStrConstructorNeg) {
    BoardSquare square = BoardSquare("-");
    EXPECT_EQ(square, BoardSquare());
}

TEST(MoveTest, MoveStrConstructor) {
    BoardMove move = BoardMove("e2e4", true);
    EXPECT_EQ(move.pos1.rank, 6);
    EXPECT_EQ(move.pos1.file, 4);
    EXPECT_EQ(move.pos2.rank, 4);
    EXPECT_EQ(move.pos2.file, 4);
}

TEST(MoveTest, MoveStrConstructor2) {
    BoardMove move = BoardMove("a7a8q", true);
    EXPECT_EQ(move.pos1.rank, 1);
    EXPECT_EQ(move.pos1.file, 0);
    EXPECT_EQ(move.pos2.rank, 0);
    EXPECT_EQ(move.pos2.file, 0);
    EXPECT_EQ(move.promotionPiece, WQueen);
}

TEST_F(BoardTest, getPieceValidSquare) {
    Board defaultBoard;
    BoardSquare square("a8");
    ASSERT_EQ(defaultBoard.getPiece(square), BRook);
}

TEST_F(BoardTest, getPieceValidSquare2) {
    Board defaultBoard;
    BoardSquare square("a7");
    ASSERT_EQ(defaultBoard.getPiece(square), BPawn);
}

TEST_F(BoardTest, getPieceValidSquare3) {
    Board defaultBoard;
    BoardSquare square("d2");
    ASSERT_EQ(defaultBoard.getPiece(square), WPawn);
}

TEST_F(BoardTest, fenConstructorDefault) {
    Board fenBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board defaultBoard;

    EXPECT_EQ(fenBoard.pieceSets, defaultBoard.pieceSets);
    EXPECT_EQ(fenBoard.board, defaultBoard.board);
    EXPECT_EQ(fenBoard.zobristKey, defaultBoard.zobristKey);
    EXPECT_NE(fenBoard.zobristKey, 0);
    EXPECT_EQ(fenBoard.isWhiteTurn, defaultBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, defaultBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, defaultBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, defaultBoard.fiftyMoveRule);
}

TEST_F(BoardTest, fenConstructorEnPassantSquare) {
    Board fenBoard("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    Board moveBoard;
    moveBoard.makeMove(BoardMove("e2e4", moveBoard.isWhiteTurn));
    
    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.zobristKey, moveBoard.zobristKey);
    EXPECT_NE(fenBoard.zobristKey, 0);
    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, moveBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, moveBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, moveBoard.fiftyMoveRule);
}

TEST_F(BoardTest, fenConstructorEnPassantCastle) {
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

    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.zobristKey, moveBoard.zobristKey);
    EXPECT_NE(fenBoard.zobristKey, 0);
    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, moveBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, moveBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, moveBoard.fiftyMoveRule);
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
    EXPECT_EQ(castleRightsBit(BoardSquare(0, G), board.isWhiteTurn) && board.castlingRights, false); // is not black's turn
    EXPECT_EQ(castleRightsBit(BoardSquare(0, C), board.isWhiteTurn) && board.castlingRights, false); // is not black's turn
    EXPECT_EQ(castleRightsBit(BoardSquare(7, G), board.isWhiteTurn) && board.castlingRights, true);
    EXPECT_EQ(castleRightsBit(BoardSquare(7, C), board.isWhiteTurn) && board.castlingRights, true);    
    EXPECT_EQ(castleRightsBit(BoardSquare(4, E), board.isWhiteTurn) && board.castlingRights, false);    
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

TEST_F(BoardTest, checkDiagAttackersTrue) {
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
    bool isAttacked = currKingInAttack(board.pieceSets, board.isWhiteTurn);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkDiagAttackersFalse) {
    Board board("7K/8/5B2/4p3/2Bk4/8/8/6p1 b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkStraightAttackersTrue) {
    Board board("7K/8/8/4p3/R2k4/8/8/6p1 b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkStraightAttackersFalse) {
    Board board("7K/8/8/2R5/2rk4/8/8/6p1 b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkKnightAttackersTrue1) {
    Board board("8/8/2N5/8/3k4/8/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkKnightAttackersTrue2) {
    Board board("8/8/8/1N6/3k4/8/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkKnightAttackersTrue3) {
    Board board("8/8/8/8/3k4/1N6/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkKnightAttackersTrue4) {
    Board board("8/8/8/8/3k4/8/2N5/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkKnightAttackersFalse) {
    Board board("8/8/8/8/3k4/8/2nN4/4N2K b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkPawnAttackersTrue1) {
    Board board("8/8/8/8/3k4/2P5/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkPawnAttackersTrue2) {
    Board board("8/8/8/8/3k4/4P3/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkPawnAttackersFalse) {
    Board board("8/8/8/2P1P3/3k4/8/8/7K b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkKingAttackersTrue1) {
    Board board("8/8/8/2K5/3k4/8/8/8 b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkKingAttackersTrue2) {
    Board board("8/8/8/2K5/3k4/8/8/8 b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, checkKingAttackersFalse) {
    Board board("8/8/8/8/3k4/8/8/7K b KQkq - 0 1");
    ASSERT_FALSE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, inCheckTrue1) {
    Board board("8/8/8/8/R2k4/8/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, inCheckTrue2) {
    Board board("8/8/1B6/8/Rr1k4/8/8/7K b KQkq - 0 1");
    ASSERT_TRUE(currKingInAttack(board.pieceSets, board.isWhiteTurn));
}

TEST_F(BoardTest, MakeMovePawnJump) {
    Board board;
    BoardSquare pos1 = BoardSquare("e2");
    BoardSquare pos2 = BoardSquare("e4");
    BoardSquare enPassantSquare = BoardSquare("e3");
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.pawnJumpedSquare, enPassantSquare);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST_F(BoardTest, MakeMoveKingCastle) {
    Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQK2R w KQkq - 0 1");
    BoardSquare pos1("e1");
    BoardSquare pos2("g1");
    BoardSquare rookStart("h1");
    BoardSquare rookEnd("f1");
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(rookStart), EmptyPiece);
    EXPECT_EQ(board.getPiece(rookEnd), WRook);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST_F(BoardTest, MakeMoveQueenCastle) {
    Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/R3KBNR w KQkq - 0 1");
    BoardSquare pos1("e1");
    BoardSquare pos2("c1");
    BoardSquare rookStart("a1");
    BoardSquare rookEnd("d1");
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(rookStart), EmptyPiece);
    EXPECT_EQ(board.getPiece(rookEnd), WRook);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST_F(BoardTest, MakeMoveMovedKing) {
    Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/R3KBNR w KQkq - 0 1");
    BoardSquare pos1 = BoardSquare("e1");
    BoardSquare pos2 = BoardSquare("e2");
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST_F(BoardTest, MakeMoveKingToCastleSquare) {
    Board board("k7/8/8/8/8/8/8/5K1R w kq - 0 1");
    BoardSquare pos1 = BoardSquare("f1");
    BoardSquare pos2 = BoardSquare("g1");
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.getPiece(7, H), WRook);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST_F(BoardTest, MakeMoveEnPassant) {
    Board fenBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    Board moveBoard("rnbqkb1r/ppp1pppp/3P1n2/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3");
    BoardSquare pos1("e5");
    BoardSquare pos2("d6"); 
    BoardSquare enemyPawn("d5");
    EXPECT_EQ(fenBoard.getPiece(enemyPawn), BPawn);
    fenBoard.makeMove(pos1, pos2);

    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(fenBoard.getPiece(pos2), WPawn);
    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, BoardSquare());
    EXPECT_EQ(fenBoard.zobristKey, moveBoard.zobristKey);
    EXPECT_EQ(fenBoard.fiftyMoveRule, 0);   
}

TEST_F(BoardTest, MakeMoveNotEnPassant) {
    Board fenBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    Board moveBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 3");
    BoardSquare pos1("g1");
    BoardSquare pos2("f3"); 
    BoardSquare enemyPawn("d5");
    EXPECT_EQ(fenBoard.getPiece(enemyPawn), BPawn);
    fenBoard.makeMove(pos1, pos2);

    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, BoardSquare());
    EXPECT_EQ(fenBoard.zobristKey, moveBoard.zobristKey);
    EXPECT_EQ(fenBoard.fiftyMoveRule, 1);   
}

TEST_F(BoardTest, MakeMovePromote) {
    Board board("8/PK6/8/8/8/8/8/7k w KQkq - 0 1");
    BoardSquare pos1 = BoardSquare("a7");
    BoardSquare pos2 = BoardSquare("a8");
    board.makeMove(pos1, pos2, WQueen);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WQueen);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST_F(BoardTest, MakeMovePawnCapture) {
    Board board("8/8/4p3/3Pp3/8/K7/8/k7 w KQkq - 0 1");
    BoardSquare pos1 = BoardSquare("d5");
    BoardSquare pos2 = BoardSquare("e6");
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(BoardSquare("e5")), BPawn);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST_F(BoardTest, LegalMoveRegularCapture) {
    Board board("8/5p2/8/3Bp3/8/K7/8/k7 w KQkq - 0 1");
    BoardSquare pos1 = BoardSquare("d5");
    BoardSquare pos2 = BoardSquare("f7");
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WBishop);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST_F(BoardTest, LegalMoveRookPin) {
    Board board("r6k/R7/8/8/8/8/8/K7 w KQkq - 0 1");
    BoardMove move("a7b7", board.isWhiteTurn);
    ASSERT_FALSE(board.isLegalMove(move));
}

TEST_F(BoardTest, LegalMoveBishopPin) {
    Board board("8/8/8/8/3b4/7k/1B6/K7 w KQkq - 0 1");
    BoardMove move("b2a3", board.isWhiteTurn);
    ASSERT_FALSE(board.isLegalMove(move));
}

TEST_F(BoardTest, LegalMoveEnPassant) {
    Board board("2RQ4/R6p/6b1/1k5p/Ppp5/8/6P1/7K b - a3 0 42");
    BoardMove move("b4a3", board.isWhiteTurn);
    ASSERT_TRUE(board.isLegalMove(move));
}

TEST_F(BoardTest, LegalMoveEnPassantPin) {
    Board board("6k1/8/7p/3K1Ppq/4n3/8/6p1/8 w - g6 0 1");
    BoardMove move("f5g6", board.isWhiteTurn);
    ASSERT_FALSE(board.isLegalMove(move));
}

TEST_F(BoardTest, MakeMoveCastleRightsRook) {
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");
    board.makeMove(BoardMove("h1g1", board.isWhiteTurn));
    EXPECT_EQ(board.castlingRights, B_Castle | W_OOO);
}

TEST_F(BoardTest, MakeMoveCastleRightsRook2) {
    Board board("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    board.makeMove(BoardMove("h8g8", board.isWhiteTurn));
    EXPECT_EQ(board.castlingRights, W_Castle | B_OOO);
}

TEST_F(BoardTest, MakeMoveCastleRightsRook3) {
    Board board("7r/1k4P1/1n6/B7/P4P1p/7P/4NK2/1R5R b - - 0 44");
    board.makeMove(BoardMove("h8g8", board.isWhiteTurn));
    EXPECT_EQ(board.castlingRights, noCastle);
}

TEST_F(BoardTest, undoMove) {
    // ruy lopez
    Board defaultBoard, moveBoard;
    moveBoard.makeMove(BoardMove("e2e4", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("e7e5", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("g1f3", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("b8c6", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("f1b5", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("g8f6", moveBoard.isWhiteTurn));
    moveBoard.makeMove(BoardMove("e1g1", moveBoard.isWhiteTurn));
    for (int i = 0; i < 7; i++) {
        moveBoard.undoMove();
    }

    EXPECT_EQ(defaultBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(defaultBoard.board, moveBoard.board);
    EXPECT_EQ(defaultBoard.zobristKey, moveBoard.zobristKey);
    EXPECT_NE(defaultBoard.zobristKey, 0);
    EXPECT_EQ(defaultBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(defaultBoard.castlingRights, moveBoard.castlingRights);
    EXPECT_EQ(defaultBoard.pawnJumpedSquare, moveBoard.pawnJumpedSquare);
    EXPECT_EQ(defaultBoard.fiftyMoveRule, moveBoard.fiftyMoveRule);
}

TEST(MaterialTest, defaultGame) {
    Board b1 = Board();
    uint8_t pieceCount1 = 0;
    uint8_t totalMaterial1 = 0;

    for(pieceTypes piece: b1.board) {
        if(piece != EmptyPiece) {
            pieceCount1++;
            totalMaterial1 += abs(pieceValues[piece]);
        }
    }
    
    b1.makeMove(BoardMove("e2e4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d7d5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("e4d5", b1.isWhiteTurn)); //cap black pawn
    int pieceCount2 = b1.eval.piecesRemaining;
    int totalMaterial2 = b1.eval.totalMaterial;
    b1.makeMove(BoardMove("c8h3", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d5d6", b1.isWhiteTurn)); 
    b1.makeMove(BoardMove("h3g2", b1.isWhiteTurn)); //cap white pawn
    int pieceCount3 = b1.eval.piecesRemaining;
    int totalMaterial3 = b1.eval.totalMaterial;
    b1.makeMove(BoardMove("d6c7", b1.isWhiteTurn)); //cap black pawn
    int pieceCount4 = b1.eval.piecesRemaining;
    int totalMaterial4 = b1.eval.totalMaterial;
    b1.makeMove(BoardMove("g2h1", b1.isWhiteTurn)); //cap white rook
    int pieceCount5 = b1.eval.piecesRemaining;
    int totalMaterial5 = b1.eval.totalMaterial;
    b1.makeMove(BoardMove("c7b8q", b1.isWhiteTurn)); //cap black knight
    int pieceCount6 = b1.eval.piecesRemaining;
    int totalMaterial6 = b1.eval.totalMaterial;

    EXPECT_EQ(pieceCount1, 32);
    EXPECT_EQ(totalMaterial1, 78);
    EXPECT_EQ(pieceCount2, 31);
    EXPECT_EQ(totalMaterial2, 77);
    EXPECT_EQ(pieceCount3, 30);
    EXPECT_EQ(totalMaterial3, 76);
    EXPECT_EQ(pieceCount4, 29);
    EXPECT_EQ(totalMaterial4, 75);
    EXPECT_EQ(pieceCount5, 28);
    EXPECT_EQ(totalMaterial5, 70);
    EXPECT_EQ(pieceCount6, 27);
    EXPECT_EQ(totalMaterial6, 75);
}

TEST(MaterialTest, enPassantTests) {
    Board b1;
    b1.makeMove(BoardMove("e2e4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d7d5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("e4e5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d5d4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("h2h3", b1.isWhiteTurn));
    b1.makeMove(BoardMove("f7f5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("e5f6", b1.isWhiteTurn)); //+1
    int pieceCount1 = b1.eval.piecesRemaining;
    int totalMaterial1 = b1.eval.totalMaterial;
    b1.makeMove(BoardMove("g7f6", b1.isWhiteTurn)); //+-0
    b1.makeMove(BoardMove("c2c4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("d4c3", b1.isWhiteTurn)); //-1
    int totalMaterial2 = b1.eval.totalMaterial;
    b1.makeMove(BoardMove("d2d4", b1.isWhiteTurn));
    b1.makeMove(BoardMove("c3b2", b1.isWhiteTurn)); //-2
    b1.makeMove(BoardMove("d4d5", b1.isWhiteTurn));
    b1.makeMove(BoardMove("b2a1r", b1.isWhiteTurn)); //-11
    int totalMaterial3 = b1.eval.totalMaterial;
    
    EXPECT_EQ(pieceCount1, 31);
    EXPECT_EQ(totalMaterial1, 77);
    EXPECT_EQ(totalMaterial2, 75);
    EXPECT_EQ(totalMaterial3, 73);
}

TEST(MaterialTest, fenBoards) {
    Board fenBoard1("3qk3/8/8/8/8/3p4/8/3QK3 w - - 0 1"); //19
    Board fenBoard2("r1bqkbnr/pp1ppppp/8/8/3QP3/8/PPP2PPP/RNB1KB1R w KQkq - 0 1"); //70

    EXPECT_EQ(fenBoard1.eval.totalMaterial, 19);
    EXPECT_EQ(fenBoard2.eval.totalMaterial, 70);
}

TEST(MoveIsCaptureTest, defaultBoard) {
    Board b1;
    BoardMove m1 = BoardMove("e2e4", b1.isWhiteTurn);
    bool b_1 = b1.moveIsCapture(m1); //F
    b1.makeMove(m1);
    BoardMove m2 = BoardMove("d7d5", b1.isWhiteTurn);
    bool b_2 = b1.moveIsCapture(m2); //F
    b1.makeMove(m2);
    BoardMove m3 = BoardMove("e4e5", b1.isWhiteTurn);
    bool b_3 = b1.moveIsCapture(m3); //F
    b1.makeMove(m3);
    BoardMove m4 = BoardMove("d5d4", b1.isWhiteTurn);
    bool b_4 = b1.moveIsCapture(m4); //F
    b1.makeMove(m4);
    BoardMove m5 = BoardMove("h2h3", b1.isWhiteTurn);
    bool b_5 = b1.moveIsCapture(m5); //F
    b1.makeMove(m5);
    BoardMove m6 = BoardMove("f7f5", b1.isWhiteTurn);
    bool b_6 = b1.moveIsCapture(m6); //F
    b1.makeMove(m6);
    BoardMove m7 = BoardMove("e5f6", b1.isWhiteTurn);
    bool b_7 = b1.moveIsCapture(m7); //T
    b1.makeMove(m7);
    BoardMove m8 = BoardMove("g7f6", b1.isWhiteTurn);
    bool b_8 = b1.moveIsCapture(m8); //T
    b1.makeMove(m8);
    BoardMove m9 = BoardMove("c2c4", b1.isWhiteTurn);
    bool b_9 = b1.moveIsCapture(m9); //F
    b1.makeMove(m9);
    BoardMove m10 = BoardMove("d4c3", b1.isWhiteTurn);
    bool b_10 = b1.moveIsCapture(m10); //T
    b1.makeMove(m10);
    BoardMove m11 = BoardMove("d2d4", b1.isWhiteTurn);
    bool b_11 = b1.moveIsCapture(m11); //F
    b1.makeMove(m11);
    BoardMove m12 = BoardMove("c3b2", b1.isWhiteTurn);
    bool b_12 = b1.moveIsCapture(m12); //T
    b1.makeMove(m12);
    BoardMove m13 = BoardMove("d4d5", b1.isWhiteTurn);
    bool b_13 = b1.moveIsCapture(m13); //F
    b1.makeMove(m13);
    BoardMove m14 = BoardMove("b2a1r", b1.isWhiteTurn);
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