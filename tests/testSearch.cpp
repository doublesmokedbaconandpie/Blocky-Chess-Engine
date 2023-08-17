#include "board.hpp"
#include "search.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <utility>


TEST(TestEval, evalTest) {
    Board board = Board();
    ASSERT_EQ(eval(board), 0);
}

TEST(TestSearch, Depth4) {
    Board board = Board();
    std::pair<int, BoardMove> returnVals = negaMax(board, 4);
    std::cout << "Evaluation: " << returnVals.first << std::endl;
    std::cout << "Best Move: " << returnVals.second << std::endl;
    std::cout << "Best Board: \n" << Board(board, returnVals.second) << std::endl;
}