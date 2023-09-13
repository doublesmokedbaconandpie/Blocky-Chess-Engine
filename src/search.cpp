#include <vector>
#include <utility>
#include <iostream>

#include "search.hpp"
#include "eval.hpp"
#include "moveGen.hpp"
#include "board.hpp"

namespace Search {
    Info Searcher::search(int depth) {
        Info result;
        Node root = this->alphaBeta(MIN_ALPHA, MAX_BETA, depth, 0);

        result.nodes = this->nodes;
        result.depth = this->depth;
        
        result.eval = root.eval;
        result.move = root.move;
        if (root.eval > MAX_BETA - 100) {
            result.mateIn = MAX_BETA - root.eval;
        }
        if (root.eval < MIN_ALPHA + 100) {
            result.mateIn = root.eval - MIN_ALPHA;
        }
        return result;
    }

    Node Searcher::alphaBeta(int alpha, int beta, int depthLeft, int distanceFromRoot) {
        Node result;
        this->nodes++;
        this->depth = distanceFromRoot > this->depth ? distanceFromRoot : this->depth;

        // fifty move rule
        if (this->board.fiftyMoveRule == 100) {
            result.eval = 0;
            return result;
        }
        // three-fold repetition
        std::vector<uint64_t> currKeyHistory = this->board.zobristKeyHistory;
        std::sort(currKeyHistory.begin(), currKeyHistory.end());
        auto lBound = std::lower_bound(currKeyHistory.begin(), currKeyHistory.end(), this->board.zobristKey);
        auto rBound = std::upper_bound(currKeyHistory.begin(), currKeyHistory.end(), this->board.zobristKey);
        if (distance(lBound, rBound) == 3) {
            result.eval = 0;
            return result;
        }
        // max depth reached
        if (depthLeft == 0) {
            result.eval = eval(this->board);
            return result;
        }
        // checkmate or stalemate
        std::vector<BoardMove> moves = MOVEGEN::moveGenerator(this->board);
        if (moves.size() == 0) {
            if (currKingInAttack(board)) {
                result.eval = MIN_ALPHA + distanceFromRoot;
            }
            else {
                result.eval = 0;
            }
            return result;
        }

        // start search through moves
        int score, bestscore = MIN_ALPHA;
        for (BoardMove move: moves) {
            board.makeMove(move);
            Node oppAlphaBeta = alphaBeta(-1 * beta, -1 * alpha, depthLeft - 1, distanceFromRoot + 1);
            board.undoMove(); 
            
            score = -1 * oppAlphaBeta.eval;
            
            // prune if a move is too good; opponent side will avoid playing into this node
            if (score >= beta) {
                result.eval = beta;
                break;
            }
            // fail-soft stabilizes the search and allows for returned.evals outside the alpha-beta bounds
            if (score > bestscore) {
                result.eval = bestscore = score;
                result.move = move;
                if (score > alpha) {
                    alpha = score;
                }
            }
        }
        return result;
    }

} // namespace Search