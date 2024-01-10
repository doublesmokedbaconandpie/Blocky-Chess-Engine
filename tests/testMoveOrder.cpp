#include "perft.hpp"
#include "attacks.hpp"

#include <gtest/gtest.h>

class MoveOrderTest : public testing::Test {
    public:
        static void SetUpTestSuite() {
            Attacks::init();
        }
};


TEST_F(MoveOrderTest, perftStartpos) {
    Board board;
    ASSERT_EQ(perftMovePicker<true>(board, 0), 1);
    ASSERT_EQ(perftMovePicker<true>(board, 1), 20);
    ASSERT_EQ(perftMovePicker<true>(board, 2), 400);
    ASSERT_EQ(perftMovePicker<true>(board, 3), 8902);
    ASSERT_EQ(perftMovePicker<true>(board, 4), 197281);
}

TEST_F(MoveOrderTest, perftKiwipete) {
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    ASSERT_EQ(perftMovePicker<true>(board, 1), 48);
    ASSERT_EQ(perftMovePicker<true>(board, 2), 2039);
    ASSERT_EQ(perftMovePicker<true>(board, 3), 97862);
    ASSERT_EQ(perftMovePicker<true>(board, 4), 4085603);
}