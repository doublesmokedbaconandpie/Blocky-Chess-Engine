#pragma once

#include <utility>

#include "board.hpp"
#include "eval.hpp"

namespace SEARCH {
    const int MIN_ALPHA = -100000000;
    const int MAX_BETA = 100000000;
    
    struct SearchInfo {
        int nodes;
        int value;
        BoardMove move = BoardMove();
    };

    SearchInfo search(Board& board, int depth);
    SearchInfo alphaBeta(Board& board, int alpha, int beta, int depth);

} // namespace SEARCH

