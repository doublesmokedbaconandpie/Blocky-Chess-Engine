#include <vector>
#include <utility>
#include <iostream>
#include <chrono>

#include "search.hpp"
#include "movePicker.hpp"
#include "eval.hpp"
#include "moveGen.hpp"
#include "board.hpp"
#include "timeman.hpp"

namespace Search {
    Info Searcher::startThinking() {
        Info result;
        Node root;

        // perform iterative deepening
        for(int i = 1; i <= this->depth_limit; i++) {
            root = this->search(MIN_ALPHA, MAX_BETA, i, 0);
            
            if(this->tm.timeUp()) {
                result.nodes = this->nodes;
                result.timeElapsed = this->tm.getTimeElapsed();
                break;
            }
            else {
                result.depth = this->max_depth;
                result.eval = root.eval;
                result.move = root.move;
            }
        }

        // compute mate-in
        if (root.eval > MAX_BETA - 100) {
            result.mateIn = MAX_BETA - root.eval;
        }
        if (root.eval < MIN_ALPHA + 100) {
            result.mateIn = root.eval - MIN_ALPHA;
        }
        return result;
    }

    Node Searcher::search(int alpha, int beta, int depthLeft, int distanceFromRoot) {
        Node result;
        if(this->tm.timeUp()) {
            return result;
        }

        this->nodes++;
        this->max_depth = distanceFromRoot > this->max_depth ? distanceFromRoot : this->max_depth;

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
            result.eval = quiesce(alpha, beta, 5);
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

        // init movePicker
        MovePicker movePicker(std::move(moves));
        movePicker.assignMoveScores(board);

        // start search through moves
        int score, bestscore = MIN_ALPHA;
        while (movePicker.movesLeft()) {
            BoardMove move = movePicker.pickMove();
            board.makeMove(move);
            Node opponent = search(-1 * beta, -1 * alpha, depthLeft - 1, distanceFromRoot + 1);
            board.undoMove(); 
            
            score = -1 * opponent.eval;
            
            // prune if a move is too good; opponent side will avoid playing into this node
            if (score >= beta) {
                result.eval = beta;
                break;
            }
            // fail-soft stabilizes the search and allows for returned values outside the alpha-beta bounds
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

    int Searcher::quiesce(int alpha, int beta, int depthLeft) {
        if(this->tm.timeUp()) {
            return -1;
        }

        this->nodes++;

        int stand_pat = this->board.getEvalScore();
        if(stand_pat >= beta)
            return beta;
        if(alpha < stand_pat)
            alpha = stand_pat;
        if(depthLeft == 0)
            return stand_pat;

        std::vector<BoardMove> moves = MOVEGEN::moveGenerator(this->board);
        MovePicker movePicker(std::move(moves));
        movePicker.assignMoveScores(board);

        int score = MIN_ALPHA;
        while (movePicker.movesLeft()) {
            BoardMove move = movePicker.pickMove();
            if(!board.moveIsCapture(move))
                continue;
            board.makeMove(move);
            score = -1 * (quiesce(-1 * beta, -1 * alpha, depthLeft - 1));
            board.undoMove(); 

            if(score >= beta)
                return beta;
            if(score > alpha)
                alpha = score;
        }
        return alpha;

    }

} // namespace Search