#pragma once

#include <utility>

#include "board.hpp"
#include "eval.hpp"

namespace Search {
    const int MIN_ALPHA = -1000000;
    const int MAX_BETA = 1000000;
    const int NO_MATE = -1;
    
    struct SearchInfo {
        int nodes;
        int eval;
        int mateIn = NO_MATE;
        BoardMove move = BoardMove();
    };
    
    struct Search {
        Search(Board board);
        SearchInfo search(int depth);
        SearchInfo alphaBeta(int alpha, int beta, int depthLeft, int distanceFromRoot);

        Board board;
        int nodes;
    };
} // namespace Search
