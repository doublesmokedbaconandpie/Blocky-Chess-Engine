#include <vector>
#include <utility>

#include "search.hpp"
#include "eval.hpp"
#include "moveGen.hpp"
#include "inCheck.hpp"
#include "board.hpp"

namespace SEARCH {
    SearchInfo search(Board& board, int depth) {
        auto result = alphaBeta(board, MIN_ALPHA, MAX_BETA, depth, 0);
        return result;
    }

    SearchInfo alphaBeta(Board& board, int alpha, int beta, int depthLeft, int distanceFromRoot) {
        SearchInfo result;
        result.nodes = 1;
        if (board.fiftyMoveRule >= 50) {
            result.value = 0;
            return result;
        }
        if (depthLeft == 0) {
            result.value = eval(board);
            return result;
        }
        std::vector<BoardMove> moves = moveGenerator(board);
        if (moves.size() == 0) {
            if (currKingInAttack(board)) {
                result.value = MIN_ALPHA + distanceFromRoot;
                result.mateIn = distanceFromRoot;
            }
            else {
                result.value = 0;
            }
            return result;
        }

        int score, bestscore = MIN_ALPHA;
        for (BoardMove move: moves) {
            Board newBoard(board, move); 
            SearchInfo oppAlphaBeta = alphaBeta(newBoard, -1 * beta, -1 * alpha, depthLeft - 1, distanceFromRoot + 1);
            result.nodes += oppAlphaBeta.nodes;
            score = -1 * oppAlphaBeta.value;

            if (score >= beta) {
                result.value = beta;
                break;
            }
            if (score > bestscore) {
                result.mateIn = oppAlphaBeta.mateIn;
                result.value = bestscore = score;
                result.move = move;
                if (score > alpha) {
                    alpha = score;
                }
            }
        }
        return result;
    }

} // namespace SEARCH