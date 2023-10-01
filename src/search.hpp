#pragma once

#include <cstdint>
#include <utility>

#include "board.hpp"
#include "eval.hpp"

namespace Search {
    const int MIN_ALPHA = -1000000;
    const int MAX_BETA = 1000000;
    const int NO_MATE = -1;

    // used for outside UCI representation    
    struct Info {
        uint64_t nodes;
        int depth;
        int eval;
        int mateIn = NO_MATE;
        BoardMove move;
    };

    // used for internal searching
    struct Node {
        int eval;
        BoardMove move;
    };
    
    class Searcher {
        public:  
            Searcher(Board a_board) {
                this->board = a_board;
                this->nodes = 0;
                this->max_depth = 0;
            }
            Info search(int depth);
            Node alphaBeta(int alpha, int beta, int depthLeft, int distanceFromRoot);
            int quiesce(int alpha, int beta, int depthLeft);
        private:
            Board board;
            uint64_t nodes;
            int max_depth;
    };
} // namespace Search
