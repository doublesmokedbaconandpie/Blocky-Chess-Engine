#pragma once

#include <string>
#include <sstream>

#include "board.hpp"

namespace UCI {
    struct UCIOPTIONS {
        bool useDepth = false;
        int depth = 0;
    };

    bool uci();

    void SETOPTIONLOOP();
    void setoption(std::istringstream& input);

    void UCILOOP();
    Board position(std::istringstream& input);
    void go(std::istringstream& input, Board& board);

    void isready();

} // namespace UCI
