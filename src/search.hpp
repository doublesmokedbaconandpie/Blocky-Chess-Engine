#pragma once

#include <cstdint>
#include <utility>
#include <chrono>

#include "board.hpp"
#include "eval.hpp"
#include "ttable.hpp"
#include "timeman.hpp"

namespace Search {

constexpr int MIN_ALPHA = -1000000;
constexpr int MAX_BETA = 1000000;
constexpr int NO_MATE = -1000000;

// used for outside UCI representation    
struct Info {
    uint64_t nodes = 0;
    int depth = 0;
    int seldepth = 0;
    int eval = 0;
    int mateIn = NO_MATE;
    BoardMove move;
    uint64_t timeElapsed = 0;
};

// used for internal searching
struct Node {
    int eval;
    BoardMove move;
};

class Searcher {
    public:  
        Searcher(Board a_board, int ms, int depthLimit) {
            this->board = a_board;
            this->nodes = 0;
            this->max_depth = 0;
            this->max_seldepth = 0;
            this->tm = Timeman::TimeManager(ms);
            this->depth_limit = depthLimit;
        };
        Info startThinking();
        Node search(int alpha, int beta, int depthLeft, int distanceFromRoot);
        int quiesce(int alpha, int beta, int depthLeft, int distanceFromRoot);
        void storeInTT(TTable::Entry entry, Node result, int distanceFromRoot);

        void outputUciInfo(Info searchResult);
    private:
        Board board;
        uint64_t nodes;
        int max_depth, max_seldepth;
        Timeman::TimeManager tm;
        int depth_limit;
};

} // namespace Search
