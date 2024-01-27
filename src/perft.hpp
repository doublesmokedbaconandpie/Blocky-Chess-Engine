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

#include <iostream>

#include "moveGen.hpp"
#include "moveOrder.hpp"
#include "board.hpp"
#include "move.hpp"

// perft is a method of determining correctness of move generators
// positions can be input and number of total leaf nodes determined
// the number determined can be compared to established values from others
template<bool printMoves>
uint64_t perft(Board& board, int depthLeft) {
    if (depthLeft == 0) {
        return 1;
    }

    MoveList gen(board);
    gen.generateAllMoves(board);
    if (depthLeft == 1 && !printMoves) {
       return gen.moves.size();
    }

    uint64_t leafNodeCount = 0;
    for (auto move: gen.moves) {
        board.makeMove(move);
        const uint64_t moveCount = perft<false>(board, depthLeft - 1);
        leafNodeCount += moveCount;
        if (printMoves) {
            std::cout << move << ": " << moveCount << std::endl; 
        }
        board.undoMove();
    }
    return leafNodeCount;
}

// This uses the movepicker instead of using the MoveList directly for testing purposes
// The movepicker is slower, which is why it's not used in actual perft for uci
template<bool printMoves>
uint64_t perftMovePicker(Board& board, int depthLeft) {
    if (depthLeft == 0) {
        return 1;
    }

    HistoryTable unusedHistory;
    MoveOrder::MovePicker movePicker(board, unusedHistory, MoveOrder::All);
    uint64_t leafNodeCount = 0;

    while (movePicker.movesLeft(board, unusedHistory)) {
        const BoardMove move = movePicker.pickMove();
        board.makeMove(move);
        const uint64_t moveCount = perftMovePicker<false>(board, depthLeft - 1);
        leafNodeCount += moveCount;
        if (printMoves) {
            std::cout << move << ": " << moveCount << std::endl;
        }
        board.undoMove();
    }
    return leafNodeCount;
}
