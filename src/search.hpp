#pragma once

#include <utility>

#include "board.hpp"
#include "eval.hpp"

namespace Search {
    const int MIN_ALPHA = -1000000;
    const int MAX_BETA = 1000000;
    const int NO_MATE = -1;

    // used for outside UCI representation    
    struct Info {
        int nodes;
        int eval;
        int mateIn = NO_MATE;
        BoardMove move;
    };

    // used for internal searching
    struct Node {
        int eval;
        BoardMove move;
    };
    
    struct Search {
        Search(Board board);
        Info search(int depth);
        Node alphaBeta(int alpha, int beta, int depthLeft, int distanceFromRoot);

        Board board;
        int nodes;
    };
} // namespace Search
