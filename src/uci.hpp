#pragma once

#include <string>
#include <sstream>

#include "board.hpp"
#include "search.hpp"

namespace Uci {
    struct UciOptions {
        int depth = 100;
    };

    bool uci();

    void setOptionLoop();
    void setOption(std::istringstream& input);

    void uciLoop();
    Board position(std::istringstream& input);
    void go(std::istringstream& input, Board& board);

    void isready();

    // for debugging
    void perft(std::istringstream& input, Board& board);

} // namespace Uci
