#pragma once

#include <iostream>

#include "moveGen.hpp"
#include "board.hpp"
#include "move.hpp"

// perft is a method of determining correctness of move generators
// positions can be input and number of total leaf nodes determined
// the number determined can be compared to established values from others
template<bool printMoves>
uint64_t perft(Board board, int depthLeft) {
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
        uint64_t moveCount = perft<false>(board, depthLeft - 1);
        leafNodeCount += moveCount;
        if (printMoves) {
            std::cout << move << ": " << moveCount << std::endl; 
        }
        board.undoMove();
    }
    return leafNodeCount;
}
