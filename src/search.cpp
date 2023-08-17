#include <limits>
#include <vector>
#include <utility>

#include "search.hpp"
#include "eval.hpp"
#include "moveGen.hpp"
#include "inCheck.hpp"
#include "board.hpp"

std::pair<int, BoardMove> negaMax(Board board, int depth) {
    if (depth == 0) {return std::pair<int, BoardMove>(eval(board), BoardMove());}
    int max = std::numeric_limits<int>::min();

    std::vector<BoardMove> moves = moveGenerator(board);
    if (moves.size() == 0) {
        if (currKingInAttackAfterMove(board)) {
            return std::pair<int, BoardMove>(max, BoardMove());
        }
        else {
            return std::pair<int, BoardMove>(0, BoardMove());
        }
    }

    BoardMove bestMove = moves.at(0);
    int score;
    for (BoardMove move: moves) {
        Board newBoard = Board(board, move); 
        score = -1 * negaMax(newBoard, depth - 1).first;
        if (score > max) {
            max = score;
            bestMove = move;
        }
    }
    return std::pair<int, BoardMove>(max, bestMove);
}
