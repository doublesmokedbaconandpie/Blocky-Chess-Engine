
#include "eval.hpp"
#include "board.hpp"

int eval(Board board) {
    int scoreSum = board.materialDifference;
    
    return board.isWhiteTurn ? scoreSum : scoreSum * -1;
}
