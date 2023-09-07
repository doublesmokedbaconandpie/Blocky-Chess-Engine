#include <vector>
#include <utility>

#include "search.hpp"
#include "eval.hpp"
#include "moveGen.hpp"
#include "board.hpp"

namespace Search {
    SearchInfo search(Board& board, int depth) {
        auto result = alphaBeta(board, MIN_ALPHA, MAX_BETA, depth, 0);
        return result;
    }

    SearchInfo alphaBeta(Board& board, int alpha, int beta, int depthLeft, int distanceFromRoot) {
        SearchInfo result;
        result.nodes = 1;

        // check for game ending by draw or checkmate
        if (board.fiftyMoveRule == 100) {
            result.value = 0;
            return result;
        }
        if (depthLeft == 0) {
            result.value = eval(board);
            return result;
        }
        std::vector<BoardMove> moves = MOVEGEN::moveGenerator(board);
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

        // start search through moves
        int score, bestscore = MIN_ALPHA;
        for (BoardMove move: moves) {
            board.makeMove(move);
            SearchInfo oppAlphaBeta = alphaBeta(board, -1 * beta, -1 * alpha, depthLeft - 1, distanceFromRoot + 1);
            board.undoMove(); 
            
            result.nodes += oppAlphaBeta.nodes;
            score = -1 * oppAlphaBeta.value;
            
            // prune if a move is too good; opponent side will avoid playing into this node
            if (score >= beta) {
                result.value = beta;
                break;
            }
            // fail-soft stabilizes the search and allows for returned values outside the alpha-beta bounds
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

} // namespace Search