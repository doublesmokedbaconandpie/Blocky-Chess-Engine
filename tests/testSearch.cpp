#include "board.hpp"
#include "search.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <utility>

TEST(TestSearch, Depth4) {
    Board board = Board();
    std::pair<int, Board> returnVals = negaMax(board, 4);
    std::cout << "laksjdfl" << returnVals.first << std::endl;
}