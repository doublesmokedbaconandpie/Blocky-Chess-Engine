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
void uciNewGame();
Board position(std::istringstream& input);
void go(std::istringstream& input, Board& board);
void isready();

// for debugging
void bench();
void perft(std::istringstream& input, Board& board);
void magics();

} // namespace Uci
