#pragma once

#include <string>
#include <sstream>

#include "board.hpp"
#include "search.hpp"

namespace UCI {
    struct UCIOPTIONS {
        int depth = 4;
    };

    bool uci();

    void SETOPTIONLOOP();
    void setoption(std::istringstream& input);

    void UCILOOP();
    Board position(std::istringstream& input);
    void go(std::istringstream& input, Board& board);
    void info(SEARCH::SearchInfo searchResult);

    void isready();

} // namespace UCI
