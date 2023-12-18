#pragma once

#include <vector>

#include "moveGen.hpp"
#include "board.hpp"
#include "move.hpp"
#include "types.hpp"

namespace MoveOrder { 

enum Stage {
    Captures = 0b01, Quiets = 0b10, All = Captures | Quiets
};

class MovePicker {
    public:
        MovePicker(const Board& board, Stage a_stage, BoardMove a_TTMove = BoardMove());
        bool movesLeft(const Board& board);
        int getMovesPicked() const;
        BoardMove pickMove();
    private:
        enum MoveScores {
            PV = 1 << 20,
            Capture = 1 << 10,
            Quiet = 0,
        };

        void assignMoveScores(const Board& board);

        MoveList moveList;
        std::vector<int> moveScores;
        Stage stage;
        BoardMove TTMove;
        unsigned int movesPicked;
};

} // namespace MoveOrder