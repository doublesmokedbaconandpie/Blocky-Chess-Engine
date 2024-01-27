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

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "bench.hpp"
#include "board.hpp"
#include "search.hpp"
#include "timeman.hpp"

namespace Bench {

uint64_t start() {
    std::cout << '\n';

    uint64_t nodeCount = 0;
    const int numPositions = fens.size();

    for (int i = 0; i < numPositions; ++i) {
        const auto fen = fens[i];
        std::cout << "Searching position " << (i + 1) << '/' << numPositions << ": " << fen << '\n';
        Search::Searcher engine(Board(fen), Timeman::TimeManager(), BENCHDEPTH);
        engine.setPrintInfo(false);
        nodeCount += engine.startThinking().nodes;
    }
    return nodeCount;
}

} // namespace Bench