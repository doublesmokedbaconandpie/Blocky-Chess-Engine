#pragma once

#include <string>
#include <sstream>

#include "board.hpp"
#include "search.hpp"

namespace Uci {
    struct UciOptions {
        int depth = 5;
    };

    bool uci();

    void setOptionLoop();
    void setOption(std::istringstream& input);

    void uciLoop();
    Board position(std::istringstream& input);
    void go(std::istringstream& input, Board& board);
    void info(Search::Info searchResult, int64_t searchDuration);

    void isready();

} // namespace Uci
