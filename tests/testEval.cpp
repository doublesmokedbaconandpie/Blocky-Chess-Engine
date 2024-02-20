#include "eval.hpp"
#include "board.hpp"
#include "move.hpp"
#include "attacks.hpp"
#include "bitboard.hpp"

#include <gtest/gtest.h>

using namespace Eval;

class EvalTest : public testing::Test {
    public:
        static void SetUpTestSuite() {
            Attacks::init();
        }
};

TEST(ScoreTest, Construction) {
    S score(1, 2);
    EXPECT_EQ(score.opScore, 1);
    EXPECT_EQ(score.egScore, 2);
};

TEST(ScoreTest, PlusEquals) {
    S score(1, 2);
    S score2(1, 2);
    score += score2;
    EXPECT_EQ(score.opScore, 2);
    EXPECT_EQ(score.egScore, 4);
};

TEST(ScoreTest, MinusEquals) {
    S score(1, 2);
    S score2(1, 3);
    score -= score2;
    EXPECT_EQ(score.opScore, 0);
    EXPECT_EQ(score.egScore, -1);
};

TEST(ScoreTest, Plus) {
    S score(1, 2);
    S score2(1, 3);
    S score3 = score + score2;
    EXPECT_EQ(score3.opScore, 2);
    EXPECT_EQ(score3.egScore, 5);
};

TEST(ScoreTest, Minus) {
    S score(1, 6);
    S score2(6, 3);
    S score3 = score - score2;
    EXPECT_EQ(score3.opScore, -5);
    EXPECT_EQ(score3.egScore, 3);
};

TEST(checkIsPassedPawn, whiteTrue) {
    int square = toSquare("d4");
    uint64_t enemyPawns = c_u64(1) << toSquare("e4") | c_u64(1) << toSquare("c3");
    ASSERT_TRUE(isPassedPawn(square, enemyPawns, true));
}

TEST(checkIsPassedPawn, whiteTrue2) {
    int square = toSquare("d4");
    uint64_t enemyPawns = c_u64(1) << toSquare("d3") | c_u64(1) << toSquare("c4");
    ASSERT_TRUE(isPassedPawn(square, enemyPawns, true));
}

TEST(checkIsPassedPawn, whiteFalse) {
    int square = toSquare("d4");
    uint64_t enemyPawns = c_u64(1) << toSquare("e4") | c_u64(1) << toSquare("c5");
    ASSERT_FALSE(isPassedPawn(square, enemyPawns, true));
}

TEST(checkIsPassedPawn, whiteFalse2) {
    int square = toSquare("d4");
    uint64_t enemyPawns = c_u64(1) << toSquare("d5") | c_u64(1) << toSquare("c3");
    ASSERT_FALSE(isPassedPawn(square, enemyPawns, true));
}

TEST(checkIsPassedPawn, blackTrue) {
    int square = toSquare("d4");
    uint64_t enemyPawns = c_u64(1) << toSquare("e4") | c_u64(1) << toSquare("c5");
    ASSERT_TRUE(isPassedPawn(square, enemyPawns, false));
}

TEST(checkIsPassedPawn, blackFalse) {
    int square = toSquare("d4");
    uint64_t enemyPawns = c_u64(1) << toSquare("e3") | c_u64(1) << toSquare("c5");
    ASSERT_FALSE(isPassedPawn(square, enemyPawns, false));
}

TEST(checkIsPassedPawn, blackFalse2) {
    int square = toSquare("b7");
    uint64_t enemyPawns = c_u64(1) << toSquare("a4") | c_u64(1) << toSquare("c4");
    ASSERT_FALSE(isPassedPawn(square, enemyPawns, false));
}

TEST(checkIsPassedPawn, blackFalse3) {
    int square = toSquare("a7");
    uint64_t enemyPawns = c_u64(1) << toSquare("a2") | c_u64(1) << toSquare("b2") | c_u64(1) << toSquare("c2");
    ASSERT_FALSE(isPassedPawn(square, enemyPawns, false));
}

TEST_F(EvalTest, bishopMobilityBlack) {
    const Board pos("rnbqk2r/pppp1ppp/5n2/4p3/1b1NP3/8/PPPP1PPP/RNBQKB1R w KQkq - 1 4");
    const Square sq = toSquare("b4");
    const auto mobilitySquares = getMobilitySquares(pos.pieceSets, false);
    const auto allPieces = pos.pieceSets.get(ALL);
    const auto mobility = getPieceMobility(BISHOP, sq, mobilitySquares, allPieces);
    ASSERT_EQ(mobility, 6);
}
