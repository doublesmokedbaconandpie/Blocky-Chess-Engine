#pragma once

#include <vector>

#include "board.hpp"
#include "move.hpp"
#include "types.hpp"

namespace MoveOrder { 

enum MoveScores {
    PV = 1 << 20,
    Capture = 1 << 10,
    Quiet = 0,
};

class MovePicker {
    public:
        MovePicker(std::vector<BoardMove>&& a_moves); 
        void assignMoveScores(const Board& board, BoardMove PVNode = BoardMove());
        bool movesLeft() const;
        BoardMove pickMove();

    private:
        std::vector<BoardMove> moves;
        std::vector<int> moveScores;
        int movesPicked;
        int size;
};

} // namespace MoveOrder