#pragma once

#include <vector>

#include "board.hpp"
#include "move.hpp"
#include "types.hpp"

class MovePicker {
    public:
        MovePicker(std::vector<BoardMove>&& a_moves); 
        void assignMoveScores(const Board& board);
        bool movesLeft() const;
        BoardMove pickMove();
    
    private:
        std::vector<BoardMove> moves;
        std::vector<int> moveScores;
        int movesPicked;
        int size;
};