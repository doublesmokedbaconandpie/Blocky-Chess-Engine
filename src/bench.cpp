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