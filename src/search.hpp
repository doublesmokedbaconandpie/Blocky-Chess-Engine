/*
* Blocky, a UCI chess engine
* Copyright (C) 2023-2024, Kevin Nguyen
*
* Blocky is free software; you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
*
* Blocky is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along with this program; 
* if not, see <https://www.gnu.org/licenses>.
*/

#pragma once

#include <cstdint>

#include "board.hpp"
#include "eval.hpp"
#include "ttable.hpp"
#include "timeman.hpp"
#include "types.hpp"

namespace Search {

constexpr int DRAW_SCORE = 0;
constexpr int INF_SCORE = 1000000;
constexpr int MATE_IN_SCORE = INF_SCORE - MAX_PLY;
constexpr int NO_SCORE = -100000000;

void initLMRTable();

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

// triangular pv-table row
struct PVRow {
    std::array<BoardMove, MAX_PLY> moves{};
    int length{};
};

struct StackEntry {
    BoardMove killerMove{};
    int ply{};
};

class Searcher {
    public:  
        Searcher(Board a_board, Timeman::TimeManager a_tm, int depthLimit) {
            this->board = a_board;
            this->tm = a_tm;
            this->depth_limit = depthLimit;
        };
        Info startThinking();
        void setPrintInfo(bool flag) {this->printInfo = flag;};
    private:
        int aspiration(int depth, int prevEval);
        template <NodeTypes NODE>
        int search(int alpha, int beta, int depth, StackEntry* ss);
        int quiesce(int alpha, int beta, StackEntry* ss);
        bool stopSearching();
        void outputUciInfo(Info searchResult) const;

        Board board;
        uint64_t nodes{};
        int max_seldepth{};

        std::array<StackEntry, MAX_PLY> stack{};
        std::array<PVRow, MAX_PLY> PVTable;
        HistoryTable history{};

        Timeman::TimeManager tm{};
        int depth_limit{};
        bool printInfo = true;
        bool stopSearchFlag = false;
};

} // namespace Search
