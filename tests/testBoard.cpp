#include "board.hpp"
#include "zobrist.hpp"

#include <gtest/gtest.h>
#include <string>
#include <stdlib.h>

class BoardTest : public testing::Test {
    public:
        static void SetUpTestSuite() {
            Zobrist::init();
        }
};

TEST_F(BoardTest, getPieceValidSquare) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(0, A);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    ASSERT_EQ(getPieceResult, BRook) << getPieceResult;
}

TEST_F(BoardTest, getPieceValidSquare2) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(1, H);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    EXPECT_EQ(getPieceResult, BPawn);
}

TEST_F(BoardTest, getPieceValidSquare3) {
    Board defaultBoard;
    BoardSquare square = BoardSquare(6, D);
    pieceTypes getPieceResult = defaultBoard.getPiece(square);
    EXPECT_EQ(getPieceResult, WPawn);
}

TEST_F(BoardTest, boardSquareStrConstructor) {
    BoardSquare square = BoardSquare("a8");
    EXPECT_EQ(square.rank, 0);
    EXPECT_EQ(square.file, 0);
}

TEST_F(BoardTest, boardSquareStrConstructorNeg) {
    BoardSquare square = BoardSquare("-");
    EXPECT_EQ(square, BoardSquare());
}

TEST_F(BoardTest, boardMoveStrConstructor) {
    BoardMove move = BoardMove("e2e4", true);
    EXPECT_EQ(move.pos1.rank, 6);
    EXPECT_EQ(move.pos1.file, 4);
    EXPECT_EQ(move.pos2.rank, 4);
    EXPECT_EQ(move.pos2.file, 4);
}

TEST_F(BoardTest, boardMoveStrConstructor2) {
    BoardMove move = BoardMove("a7a8q", true);
    EXPECT_EQ(move.pos1.rank, 1);
    EXPECT_EQ(move.pos1.file, 0);
    EXPECT_EQ(move.pos2.rank, 0);
    EXPECT_EQ(move.pos2.file, 0);
    EXPECT_EQ(move.promotionPiece, WQueen);
}

TEST_F(BoardTest, fenConstructorDefault) {
    Board fenBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board defaultBoard;

    EXPECT_EQ(fenBoard.pieceSets, defaultBoard.pieceSets);
    EXPECT_EQ(fenBoard.board, defaultBoard.board);
    EXPECT_EQ(fenBoard.zobristKey, defaultBoard.zobristKey);
    EXPECT_NE(fenBoard.zobristKey, 0);
    EXPECT_EQ(fenBoard.zobristKeyHistory.back(), fenBoard.zobristKey);
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
    EXPECT_EQ(fenBoard.zobristKeyHistory.back(), fenBoard.zobristKey);
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
    EXPECT_EQ(fenBoard.zobristKeyHistory.back(), fenBoard.zobristKey);
    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.castlingRights, moveBoard.castlingRights);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, moveBoard.pawnJumpedSquare);
    EXPECT_EQ(fenBoard.fiftyMoveRule, moveBoard.fiftyMoveRule);
}

TEST_F(BoardTest, toFenDefault) {
    std::string expectedFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Board board(expectedFen);

    EXPECT_EQ(board.toFen(), expectedFen);
}

TEST_F(BoardTest, toFenMoves) {
    std::string expectedFen = "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 1";
    Board board(expectedFen);

    EXPECT_EQ(board.toFen(), expectedFen);
}

TEST_F(BoardTest, toFenCastling) {
    std::string expectedFen = "r1b1kbnr/ppppqppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 1";
    Board board(expectedFen);

    EXPECT_EQ(board.toFen(), expectedFen);
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
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkDiagAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WBishop   , BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, false);
}

TEST_F(BoardTest, checkStraightAttackersTrue) {
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
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}


TEST_F(BoardTest, checkStraightAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BRook     , BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, false);
}

TEST_F(BoardTest, checkKnightAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkKnightAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkKnightAttackersTrue3) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkKnightAttackersTrue4) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkKnightAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BKnight   , WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, false);
}

TEST_F(BoardTest, checkPawnAttackersTrue1) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkPawnAttackersTrue2) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkPawnAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, false);
}

TEST_F(BoardTest, checkKingAttackersTrue1) {
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
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkKingAttackersTrue2) {
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
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, checkKingAttackersFalse) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, false);
}

TEST_F(BoardTest, inCheckTrue1) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, inCheckTrue2) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , BRook     , EmptyPiece, BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     ,
    };
    Board board(boardArr, false);
    bool isAttacked = currKingInAttack(board);
    ASSERT_EQ(isAttacked, true);
}

TEST_F(BoardTest, BoardMoveConstructorPawnJump) {
    Board board;
    BoardSquare pos1 = BoardSquare(6, E);
    BoardSquare pos2 = BoardSquare(4, E);
    BoardSquare enPassantSquare = BoardSquare(5, E);
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.pawnJumpedSquare, enPassantSquare);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST_F(BoardTest, BoardMoveConstructorKingCastle) {
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
    Board board(boardArr, true);
    BoardSquare pos1 = BoardSquare(7, E);
    BoardSquare pos2 = BoardSquare(7, G);
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, H), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, F), WRook);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST_F(BoardTest, BoardMoveConstructorQueenCastle) {
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
    Board board(boardArr, true);
    BoardSquare pos1 = BoardSquare(7, E);
    BoardSquare pos2 = BoardSquare(7, C);
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, A), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, D), WRook);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST_F(BoardTest, BoardMoveConstructorMovedKing) {
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
    Board board(boardArr, true);
    BoardSquare pos1 = BoardSquare(7, E);
    BoardSquare pos2 = BoardSquare(6, E);
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(7, A), WRook);
    EXPECT_EQ(board.getPiece(7, D), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST_F(BoardTest, BoardMoveConstructorKingToCastleSquare) {
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
    Board board(boardArr, true);
    board.castlingRights = B_Castle;
    BoardSquare pos1 = BoardSquare(7, F);
    BoardSquare pos2 = BoardSquare(7, G);
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WKing);
    EXPECT_EQ(board.getPiece(7, H), WRook);
    EXPECT_EQ(board.fiftyMoveRule, 1);
    EXPECT_EQ(board.castlingRights, B_Castle);
}

TEST_F(BoardTest, BoardMoveConstructorEnPassant) {
    Board fenBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    Board moveBoard("rnbqkb1r/ppp1pppp/3P1n2/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3");
    BoardSquare pos1("e5");
    BoardSquare pos2("d6"); 
    BoardSquare enemyPawn("d5");
    EXPECT_EQ(fenBoard.getPiece(enemyPawn), BPawn);
    fenBoard.makeMove(pos1, pos2);

    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.isIllegalPos, moveBoard.isIllegalPos);
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(fenBoard.getPiece(pos2), WPawn);
    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, BoardSquare());
    EXPECT_EQ(fenBoard.zobristKey, moveBoard.zobristKey);
    EXPECT_EQ(fenBoard.fiftyMoveRule, 0);   
}

TEST_F(BoardTest, BoardMoveConstructorNotEnPassant) {
    Board fenBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    Board moveBoard("rnbqkb1r/ppp1pppp/5n2/3pP3/8/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 3");
    BoardSquare pos1("g1");
    BoardSquare pos2("f3"); 
    BoardSquare enemyPawn("d5");
    EXPECT_EQ(fenBoard.getPiece(enemyPawn), BPawn);
    fenBoard.makeMove(pos1, pos2);

    EXPECT_EQ(fenBoard.isWhiteTurn, moveBoard.isWhiteTurn);
    EXPECT_EQ(fenBoard.isIllegalPos, moveBoard.isIllegalPos);
    EXPECT_EQ(fenBoard.board, moveBoard.board);
    EXPECT_EQ(fenBoard.pieceSets, moveBoard.pieceSets);
    EXPECT_EQ(fenBoard.pawnJumpedSquare, BoardSquare());
    EXPECT_EQ(fenBoard.zobristKey, moveBoard.zobristKey);
    EXPECT_EQ(fenBoard.fiftyMoveRule, 1);   
}

TEST_F(BoardTest, BoardMoveConstructorPromote) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WPawn     , WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BKing     ,
    };
    Board board(boardArr, true);
    BoardSquare pos1 = BoardSquare(1, A);
    BoardSquare pos2 = BoardSquare(0, A);
    board.makeMove(pos1, pos2, WQueen);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WQueen);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST_F(BoardTest, BoardMoveConstructorPawnCapture) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, true);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(2, E);
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WPawn);
    EXPECT_EQ(board.getPiece(3, E), BPawn);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST_F(BoardTest, BoardMoveConstructorRegularCapture) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        BKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, true);
    BoardSquare pos1 = BoardSquare(3, D);
    BoardSquare pos2 = BoardSquare(1, F);
    board.makeMove(pos1, pos2);

    EXPECT_EQ(board.isWhiteTurn, false);
    EXPECT_EQ(board.isIllegalPos, false);
    EXPECT_EQ(board.getPiece(pos1), EmptyPiece);
    EXPECT_EQ(board.getPiece(pos2), WBishop);
    EXPECT_EQ(board.fiftyMoveRule, 0);
}

TEST_F(BoardTest, BoardMoveConstructorRookPin) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        BRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BKing     ,
        WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, true);
    BoardSquare pos1 = BoardSquare(1, A);
    BoardSquare pos2 = BoardSquare(1, B);
    board.makeMove(pos1, pos2);
    ASSERT_EQ(board.isIllegalPos, true);
}

TEST_F(BoardTest, BoardMoveConstructorBishopPin) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, BBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BKing     ,
        EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board(boardArr, true);
    BoardSquare pos1 = BoardSquare(6, B);
    BoardSquare pos2 = BoardSquare(5, A);
    board.makeMove(pos1, pos2);
    ASSERT_EQ(board.isIllegalPos, true);
}

TEST_F(BoardTest, BoardMoveConstructorCastleRightsRook) {
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");
    board.makeMove(BoardMove("h1g1", board.isWhiteTurn));
    EXPECT_EQ(board.castlingRights, B_Castle | W_OOO);
}

TEST_F(BoardTest, BoardMoveConstructorCastleRightsRook2) {
    Board board("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    board.makeMove(BoardMove("h8g8", board.isWhiteTurn));
    EXPECT_EQ(board.castlingRights, W_Castle | B_OOO);
}

TEST_F(BoardTest, BoardMoveConstructorCastleRightsRook3) {
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
    EXPECT_EQ(defaultBoard.zobristKeyHistory.back(), defaultBoard.zobristKey);
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

//NOTE
//
//As the piece-square tables are altered, these tests will begin to fail. 
//If you are still concerned about the functions working at all, then adjust the expected values accordingly.
//Otherwise, you can ignore these tests.
TEST(PlacementScoreTest, defaultBoard) {
    Board b1 = Board(); 
    
    int placementScore0 = b1.eval.placementScore;

    b1.makeMove(BoardMove("e2e4", b1.isWhiteTurn)); //-5 to +15
    int placementScore1 = b1.eval.placementScore;   //+20
    b1.makeMove(BoardMove("d7d5", b1.isWhiteTurn)); //-5 to +15
    b1.makeMove(BoardMove("e4e5", b1.isWhiteTurn)); //+15 to +20
    b1.makeMove(BoardMove("d5d4", b1.isWhiteTurn)); //+15 to +20
    b1.makeMove(BoardMove("h2h3", b1.isWhiteTurn)); //0 to 0
    b1.makeMove(BoardMove("f7f5", b1.isWhiteTurn)); //0 to 6
    int placementScore2 = b1.eval.placementScore;   //-6
    b1.makeMove(BoardMove("e5f6", b1.isWhiteTurn)); //White +20 to +15, Black -6
    int placementScore3 = b1.eval.placementScore;   //-5
    b1.makeMove(BoardMove("g7f6", b1.isWhiteTurn)); //White -15, Black 7 to 3
    int placementScore4 = b1.eval.placementScore;   //-16
    b1.makeMove(BoardMove("c2c4", b1.isWhiteTurn)); //0 to 6 (-10)
    b1.makeMove(BoardMove("d4c3", b1.isWhiteTurn)); //White -6, Black 20 to 15
    int placementScore5 = b1.eval.placementScore;   //-11
    b1.makeMove(BoardMove("d2d4", b1.isWhiteTurn)); //-5 to 15 (+9)
    b1.makeMove(BoardMove("c3b2", b1.isWhiteTurn)); //White -7, Black 15 to 20
    int placementScore6 = b1.eval.placementScore;   //-3
    b1.makeMove(BoardMove("d4d5", b1.isWhiteTurn)); //15 to 20 (+2)
    b1.makeMove(BoardMove("b2a1n", b1.isWhiteTurn)); //White 0, Black 20 to -20 (+42)
    int placementScore7 = b1.eval.placementScore;

    EXPECT_EQ(placementScore0, 0);
    EXPECT_EQ(placementScore1, 20);
    EXPECT_EQ(placementScore2, -6);
    EXPECT_EQ(placementScore3, -5);
    EXPECT_EQ(placementScore4, -16);
    EXPECT_EQ(placementScore5, -11);
    EXPECT_EQ(placementScore6, -3);
    EXPECT_EQ(placementScore7, 42);
}

TEST(PlacementScoreTest, fenBoards) {
    Board fenBoard1 = Board("4k3/8/8/3pp3/8/8/8/R3K2R w KQ - 0 1");
    Board fenBoard2 = Board("4k3/8/1q1n4/3pp3/8/2B5/1P6/2KR4 w - - 0 1");


    EXPECT_EQ(fenBoard1.eval.placementScore, -30);
    EXPECT_EQ(fenBoard2.eval.placementScore, -13);
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