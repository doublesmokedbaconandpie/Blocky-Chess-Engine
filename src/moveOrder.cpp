#include <cassert>
#include <vector>
#include <iostream>

#include "moveOrder.hpp"
#include "board.hpp"
#include "move.hpp"
#include "types.hpp"

namespace MoveOrder {

MovePicker::MovePicker(const Board& board, Stage a_stage, BoardMove a_TTMove) {
    this->moveList = MoveList(board);
    this->moveScores = std::array<int, MAX_MOVES>{};
    this->TTMove = a_TTMove;
    this->movesPicked = 0;

    if (board.moveIsCapture(this->TTMove)) {
        this->moveList.generateCaptures(board);
        this->assignMoveScores<true, Stage::Captures>(board);
        this->stage = a_stage & Stage::Quiets;
    } 
    else if (a_stage == Stage::Captures){
        this->moveList.generateCaptures(board);
        this->assignMoveScores<false, Stage::Captures>(board);
        this->stage = Stage::None;
    }
    else {
        this->moveList.generateCaptures(board);
        this->moveList.generateQuiets(board);
        this->assignMoveScores<true, Stage::All>(board);
        this->stage = Stage::None;
    }
}

// Searching moves that are likely to be better helps with pruning in search. This is move ordering.
// More promising moves are given higher scores and then searched first.
template<bool ASSIGN_TTMOVE, Stage STAGE>
void MovePicker::assignMoveScores(const Board& board) {
    constexpr bool ASSIGN_CAPTURES = STAGE & Stage::Captures;

    for (size_t i = this->movesPicked; i < this->moveList.moves.size(); ++i) {
        auto move = this->moveList.moves[i];
        if (ASSIGN_TTMOVE && move == this->TTMove) {
            this->moveScores[i] = MoveScores::PV;
        }
        else if (ASSIGN_CAPTURES && board.moveIsCapture(move)) {
            int attackerValue = pieceValues[board.getPiece(move.sqr1())];
            int victimValue = pieceValues[board.getPiece(move.sqr2())];
            this->moveScores[i] = MoveScores::Capture + victimValue - attackerValue;
        }
        else {
            this->moveScores[i] = MoveScores::Quiet;
        }
    }
}

bool MovePicker::movesLeft(const Board& board) {
    // check if an additional quiet stage needs to be generated
    if (this->movesPicked >= this->moveList.moves.size() && this->stage == Stage::Quiets) {
        this->moveList.generateQuiets(board);
        this->assignMoveScores<false, Stage::Quiets>(board);
        this->stage = Stage::None;
    }
    return this->movesPicked < this->moveList.moves.size();
}

int MovePicker::getMovesPicked() const {
    return this->movesPicked;
}

// Due to pruning, we don't need to sort the entire array of moves for move ordering.
// When sorting only small portions of arrays, using partial insertion sort is faster.
BoardMove MovePicker::pickMove() {
    assert(this->movesPicked < this->moveList.moves.size());

    auto begin = this->moveScores.begin() + this->movesPicked;
    auto end = this->moveScores.begin() + this->moveList.moves.size();
    size_t maxIndex = std::distance(this->moveScores.begin(), std::max_element(begin, end));
    
    std::swap(this->moveScores[maxIndex], this->moveScores[movesPicked]);
    std::swap(this->moveList.moves[maxIndex], this->moveList.moves[movesPicked]);
    BoardMove move = this->moveList.moves[movesPicked];
    
    ++this->movesPicked;
    return move;
}

} // namespace MoveOrder