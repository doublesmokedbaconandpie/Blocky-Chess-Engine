#pragma once

#include <utility>

#include "board.hpp"
#include "eval.hpp"

namespace SEARCH {
    const int MIN_ALPHA = -1000000;
    const int MAX_BETA = 1000000;
    const int NO_MATE = -1;
    
    struct SearchInfo {
        int nodes;
        int value;
        int mateIn = NO_MATE;
        BoardMove move = BoardMove();
    };

    SearchInfo search(Board& board, int depth);
    SearchInfo alphaBeta(Board& board, int alpha, int beta, int depthLeft, int distanceFromRoot);

} // namespace SEARCH

