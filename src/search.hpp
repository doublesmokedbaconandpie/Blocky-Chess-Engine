#pragma once

#include "board.hpp"
#include "eval.hpp"

std::pair<int, Board> negaMax(Board board, int depth);