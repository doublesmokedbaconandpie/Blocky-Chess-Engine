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
    this->stage = a_stage;
    this->TTMove = a_TTMove;
    this->movesPicked = 0;
}

// Searching moves that are likely to be better helps with pruning in search. This is move ordering.
// More promising moves are given higher scores and then searched first.
void MovePicker::assignMoveScores(const Board& board) {
    for (size_t i = this->movesPicked; i < this->moveList.moves.size(); ++i) {
        auto move = this->moveList.moves[i];
        if (move == this->TTMove) {
            this->moveScores[i] = MoveScores::PV;
        }
        else if (board.moveIsCapture(move)) {
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
    // check if an additional stage needs to be generated
    if (this->movesPicked >= this->moveList.moves.size()) {
        int oldSize = this->moveList.moves.size();
        if (this->stage & Stage::Captures) {
            this->moveList.generateCaptures(board);
            this->stage ^= Stage::Captures;
        }
        else if (this->stage & Stage::Quiets) {
            this->moveList.generateQuiets(board);
            this->stage ^= Stage::Quiets;
        }
        int newSize = this->moveList.moves.size();

        // if there are moves left after generating, reserve memory for moveScores and assign them
        if (this->movesPicked < newSize) {
            std::vector<int> newMoves(newSize - oldSize);
            this->moveScores.insert(moveScores.end(), newMoves.begin(), newMoves.end());
            assert(this->moveScores.size() == this->moveList.moves.size());
            this->assignMoveScores(board);
            return true;
        }
        return false;
    }
    return this->movesPicked < this->moveList.moves.size();
}


int MovePicker::getMovesPicked() const {
    return this->movesPicked;
}

// Due to pruning, we don't need to sort the entire array of moves for move ordering.
// When sorting only small portions of arrays, using partial insertion sort is faster.
BoardMove MovePicker::pickMove() {
    auto begin = this->moveScores.begin() + this->movesPicked;
    auto end = this->moveScores.end();
    size_t maxIndex = std::distance(this->moveScores.begin(), std::max_element(begin, end));
    
    std::swap(this->moveScores[maxIndex], this->moveScores[movesPicked]);
    std::swap(this->moveList.moves[maxIndex], this->moveList.moves[movesPicked]);
    BoardMove move = this->moveList.moves[movesPicked];
    
    ++this->movesPicked;
    return move;
}

} // namespace MoveOrder