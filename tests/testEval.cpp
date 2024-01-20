#include "eval.hpp"

#include <gtest/gtest.h>

TEST(ScoreTest, Construction) {
    Eval::S score(1, 2);
    EXPECT_EQ(score.opScore, 1);
    EXPECT_EQ(score.egScore, 2);
};

TEST(ScoreTest, PlusEquals) {
    Eval::S score(1, 2);
    Eval::S score2(1, 2);
    score += score2;
    EXPECT_EQ(score.opScore, 2);
    EXPECT_EQ(score.egScore, 4);
};

TEST(ScoreTest, MinusEquals) {
    Eval::S score(1, 2);
    Eval::S score2(1, 3);
    score += score2;
    EXPECT_EQ(score.opScore, 0);
    EXPECT_EQ(score.egScore, -1);
};

TEST(ScoreTest, Plus) {
    Eval::S score(1, 2);
    Eval::S score2(1, 3);
    Eval::S score3 = score + score2;
    EXPECT_EQ(score3.opScore, 2);
    EXPECT_EQ(score3.egScore, 5);
};

TEST(ScoreTest, Minus) {
    Eval::S score(1, 6);
    Eval::S score2(6, 3);
    Eval::S score3 = score - score2;
    EXPECT_EQ(score3.opScore, -5);
    EXPECT_EQ(score3.egScore, 3);
};