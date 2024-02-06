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

#include <array>
#include <vector>

#include "moveGen.hpp"
#include "board.hpp"
#include "move.hpp"

namespace MoveOrder { 

enum Stage {
    None = 0, Captures = 0b01, Quiets = 0b10, All = Captures | Quiets
};

class MovePicker {
    public:
        MovePicker(const Board& board, const HistoryTable& history, Stage a_stage, BoardMove a_TTMove = BoardMove(), BoardMove a_killerMove = BoardMove());
        bool movesLeft(const Board& board, const HistoryTable& history);
        int getMovesPicked() const;
        bool stagesLeft() const;
        BoardMove pickMove();
    private:
        enum MoveScores {
            PV = 1 << 30,
            Capture = 1 << 25,
            Killer = 1 << 20,
            Quiet = 0,
        };

        template<bool ASSIGN_TTMOVE, Stage STAGE>
        void assignMoveScores(const Board& board, const HistoryTable& history);
        int getVictimScore(const Board& board, BoardMove move) const;

        MoveList moveList;
        std::array<int, MAX_MOVES> moveScores;
        Stage stage;
        BoardMove TTMove, killerMove;
        unsigned int movesPicked;
};

constexpr inline Stage operator&(Stage lhs, Stage rhs) {
    return static_cast<Stage>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

} // namespace MoveOrder