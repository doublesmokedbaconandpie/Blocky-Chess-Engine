#include "board.hpp"
#include "moveGen.hpp"
#include "perft.hpp"
#include "attacks.hpp"
#include "bitboard.hpp"
#include "zobrist.hpp"

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>

class MoveGenTest : public testing::Test {
    public:
        static void SetUpTestSuite() {
            Attacks::init();
        }
};

TEST_F(MoveGenTest, validPawnMovesCaptures) {
    Board board("7k/8/8/p1p14/KP1P4/8/6P1/8 w - - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 11);
}

TEST_F(MoveGenTest, validPawnMovesPromotion) {
    Board board("8/3P4/3K4/8/8/8/8/7k w - - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 11);
}

TEST_F(MoveGenTest, validKnightMoves1) {
    Board board("7k/8/8/8/3N4/8/2n1K3/N7 w - - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 15);
}

TEST_F(MoveGenTest, validRookMoves1) {
    Board board("BB5k/8/8/8/8/8/2K5/RR5r w - - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 37);
}

TEST_F(MoveGenTest, validBishopMoves1) {
    Board board("7k/8/5b2/8/3B4/8/8/BK6 w - - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 16);
}

TEST_F(MoveGenTest, validQueenMoves1) {
    Board board("3k4/8/8/8/8/8/2K5/Q6Q w - - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 48);
}

TEST_F(MoveGenTest, validKingMovesNoCastle) {
    Board board("7k/8/8/8/8/4p3/8/4K3 w - - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 3);
}

TEST_F(MoveGenTest, validKingMovesKingCastle) {
    Board board("4k3/8/8/8/8/8/8/R1B1K2R w KQ - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 30);
}

TEST_F(MoveGenTest, validKingMovesQueenCastle) {
    Board board("4k3/8/8/8/8/8/8/R3K1BR w KQ - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 30);
}

TEST_F(MoveGenTest, validKingMovesInvalidCastle) {
    Board board("3qk3/8/8/8/8/8/8/R3K1BR w KQkq - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 27);
}

TEST_F(MoveGenTest, moveGeneratorDefault) {
    Board board;
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 20);
}

TEST_F(MoveGenTest, validKingMovesInvalidCastle2) {
    Board board("r2rk3/p4p1p/2p4b/2Q5/1P1Nq3/P1B2P1b/2P4P/R2K3R b q - 0 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 43);
}

TEST_F(MoveGenTest, validKingMovesValidCastle3) {
    Board board("r3k3/pp1r1p1p/2B4b/5Q2/1P1Nq3/P1B4b/2P2P1P/R2K3R b q - 2 1");
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 42);
}

TEST_F(MoveGenTest, moveGeneratorBlackMove) {
    Board board;
    board.makeMove(Move(toSquare("e2"), toSquare("e3")));
    MoveList moveGen(board);
    moveGen.generateAllMoves(board);
    ASSERT_EQ(moveGen.moves.size(), 20);
}

TEST_F(MoveGenTest, perftStartpos) {
    Board board;
    ASSERT_EQ(perft<true>(board, 0), 1);
    ASSERT_EQ(perft<true>(board, 1), 20);
    ASSERT_EQ(perft<true>(board, 2), 400);
    ASSERT_EQ(perft<true>(board, 3), 8902);
    ASSERT_EQ(perft<true>(board, 4), 197281);
}

TEST_F(MoveGenTest, perftKiwipete) {
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    ASSERT_EQ(perft<true>(board, 1), 48);
    ASSERT_EQ(perft<true>(board, 2), 2039);
    ASSERT_EQ(perft<true>(board, 3), 97862);
    ASSERT_EQ(perft<true>(board, 4), 4085603);
}