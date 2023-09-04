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
    GTEST_SKIP();
    Board board = Board();
    Search::SearchInfo returnVals = Search::search(board, 1);
    board.makeMove(returnVals.move);
    
    std::cout << "Evaluation: " << returnVals.value << std::endl;
    std::cout << "Best Move: " << returnVals.move << std::endl;
    std::cout << "Best Board: \n" << board << std::endl;
}