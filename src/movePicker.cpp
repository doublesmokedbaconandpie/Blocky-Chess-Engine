#include <vector>

#include "movePicker.hpp"
#include "board.hpp"
#include "move.hpp"
#include "types.hpp"

MovePicker::MovePicker(std::vector<BoardMove>&& a_moves) {
    this->moves = a_moves;
    this->moveScores = std::vector<int>(moves.size());
    this->size = moves.size();
    this->movesPicked = 0;
};

// move score assignment is implemented that more promising moves are given higher scores
// regular moves have a score of 0
void MovePicker::assignMoveScores(const Board& board) {
    size_t i = 0;
    for (BoardMove move: this->moves) {
        // capture
        // moveGen outputs least valuable piece moves first, so least value captures is automatic 
        if (board.getPiece(move.pos2) != EmptyPiece) {
            this->moveScores[i] = 1;
        }
        // default
        else {
            this->moveScores[i] = 0;
        }
        i++;
    }
}

bool MovePicker::movesLeft() const {
    return this->movesPicked < this->size;
}

// insertion sort is used to track picked moves because it is faster for very small sorted arrays
// small sorted arrays can be expected with more pruning from search
BoardMove MovePicker::pickMove() {
    auto begin = this->moveScores.begin() + this->movesPicked;
    auto end = this->moveScores.end();
    size_t maxIndex = std::distance(this->moveScores.begin(), std::max_element(begin, end));
    
    std::swap(this->moveScores[maxIndex], this->moveScores[movesPicked]);
    std::swap(this->moves[maxIndex], this->moves[movesPicked]);
    BoardMove move = this->moves[movesPicked];
    
    this->movesPicked++;
    return move;
}