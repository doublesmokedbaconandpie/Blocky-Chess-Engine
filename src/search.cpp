#include <limits>
#include <vector>
#include <utility>

#include "search.hpp"
#include "eval.hpp"
#include "moveGen.hpp"
#include "board.hpp"

std::pair<int, Board> negaMax(Board board, int depth) {
    if (depth == 0) {return std::pair<int, Board>(eval(board), board);}
    int score;
    int max = std::numeric_limits<double>::min();
    Board bestBoard = Board();
    std::vector<Board> moves = moveGenerator(board);
    for (Board move: moves) {
        score = -1 * negaMax(move, depth - 1).first;
        if (score > max) {
            max = score;
            bestBoard = move;
        }
    }
    return std::pair<int, Board>(max, bestBoard);
}
