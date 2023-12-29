#pragma once

#include <cstdint>

#include "board.hpp"
#include "eval.hpp"
#include "ttable.hpp"
#include "timeman.hpp"

namespace Search {

constexpr int DRAW_SCORE = 0;
constexpr int MIN_ALPHA = -1000000;
constexpr int MAX_BETA = 1000000;
constexpr int NO_SCORE = -100000000;

// used for outside UCI representation    
struct Info {
    uint64_t nodes{};
    int depth{};
    int seldepth{};
    int eval{};
    int mateIn = NO_SCORE;
    BoardMove move{};
    uint64_t timeElapsed{};
};

struct StackEntry {
    int distanceFromRoot;
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
        int search(int alpha, int beta, int depth, StackEntry* ss);
        int quiesce(int alpha, int beta, int depth, StackEntry* ss);
        void storeInTT(TTable::Entry entry, int eval, BoardMove move, int depth) const;

        void outputUciInfo(Info searchResult) const;
        void setPrintInfo(bool flag) {this->printInfo = flag;}; 

    private:
        Board board;
        uint64_t nodes;
        int max_seldepth;
        BoardMove finalMove;

        std::array<StackEntry, MAX_PLY> stack;
        Timeman::TimeManager tm;
        int depth_limit;
        bool printInfo = true;
};

} // namespace Search
