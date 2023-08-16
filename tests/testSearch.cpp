#include "board.hpp"
#include "search.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <utility>

TEST(TestSearch, Depth4) {
    Board board = Board();
    std::pair<int, Board> returnVals = negaMax(board, 2);
    std::cout << "Evaluation: " << returnVals.first << std::endl;
    std::cout << "Best Move: " << returnVals.second << std::endl;
}