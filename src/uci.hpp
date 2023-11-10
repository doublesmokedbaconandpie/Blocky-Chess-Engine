#pragma once

#include <string>
#include <sstream>

#include "board.hpp"
#include "search.hpp"

namespace Uci {

struct UciOptions {
    int depth = 100;
};

void printEngineInfo();
void setOptionLoop();
void uciLoop();

void setOption(std::istringstream& input);
Board position(std::istringstream& input);
void go(std::istringstream& input, Board& board);
void isready();

// for debugging
void perft(std::istringstream& input, Board& board);

} // namespace Uci
