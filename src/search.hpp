#pragma once

#include <cstdint>

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

class Searcher {
    public:  
        Searcher(Board a_board, Timeman::TimeManager a_tm, int depthLimit) {
            this->board = a_board;
            this->nodes = 0;
            this->max_seldepth = 0;
            this->tm = a_tm;
            this->depth_limit = depthLimit;
        };
        Info startThinking();

        template <NodeTypes NODE>
        int search(int alpha, int beta, int depth, int distanceFromRoot);
        int quiesce(int alpha, int beta, int depth, int distanceFromRoot);
        void storeInTT(TTable::Entry entry, int eval, BoardMove move, int depth);

        void outputUciInfo(Info searchResult);
        void setPrintInfo(bool flag) {this->printInfo = flag;}; 
    private:
        Board board;
        uint64_t nodes;
        int max_seldepth;
        BoardMove finalMove;

        Timeman::TimeManager tm;
        int depth_limit;
        bool printInfo = true;
};

} // namespace Search
