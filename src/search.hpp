#pragma once

#include <utility>

#include "board.hpp"
#include "eval.hpp"

std::pair<int, BoardMove> negaMax(Board board, int depth);