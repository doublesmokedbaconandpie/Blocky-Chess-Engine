#include <vector>
#include <utility>
#include <iostream>
#include <chrono>

#include "search.hpp"
#include "ttable.hpp"
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
            result.nodes = this->nodes;
            result.timeElapsed = this->tm.getTimeElapsed();
            // only update the following results if search successfully checked a move in time
            if (root.move != BoardMove()) {
                result.move = root.move;
                result.depth = this->max_depth;
                result.seldepth = this->max_seldepth;
                result.eval = root.eval;
                // compute mate-in
                if (abs(result.eval) > MAX_BETA - 100) {
                    int colorToMate = result.eval < 0 ? -1 : 1;
                    result.mateIn = colorToMate * (MAX_BETA - abs(result.eval));
                }
                this->outputUciInfo(result);
            }
            
            if(this->tm.timeUp()) {
                break;
            }
        }

        return result;
    }

    Node Searcher::search(int alpha, int beta, int depthLeft, int distanceFromRoot) {
        Node result;
        if(this->tm.timeUp()) {
            return result;
        }

        ++this->nodes;
        this->max_depth = std::max(distanceFromRoot, this->max_depth);

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
            result.eval = quiesce(alpha, beta, 5, distanceFromRoot);
            return result;
        }
        // checkmate or stalemate
        std::vector<BoardMove> moves = MoveGen::moveGenerator(this->board);
        if (moves.size() == 0) {
            if (currKingInAttack(board.pieceSets, board.isWhiteTurn)) {
                result.eval = MIN_ALPHA + distanceFromRoot;
            }
            else {
                result.eval = 0;
            }
            return result;
        }

        // probe transposition table for PVNodes, which help with move ordering
        BoardMove PVNode;
        TTable::Entry entry;
        int posIndex = TTable::table.getIndex(this->board.zobristKey);
        if (TTable::table.entryExists(this->board.zobristKey)) {
            entry = TTable::table.getEntry(posIndex);
            PVNode = entry.move;
        }

        // init movePicker
        MovePicker movePicker(std::move(moves));
        movePicker.assignMoveScores(board, PVNode);

        // start search through moves
        int score, bestscore = MIN_ALPHA;
        while (movePicker.movesLeft()) {
            BoardMove move = movePicker.pickMove();
            board.makeMove(move);
            Node opponent = search(-1 * beta, -1 * alpha, depthLeft - 1, distanceFromRoot + 1);
            board.undoMove(); 
            
            score = -1 * opponent.eval;

            // don't update best move if time is up
            if (this->tm.timeUp()) {
                break;
            }
            
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
        // A search for this depth is complete with a best move, so it can be stored in the transposition table
        this->storeInTT(entry, result, distanceFromRoot);
        return result;
    }

    int Searcher::quiesce(int alpha, int beta, int depthLeft, int distanceFromRoot) {
        if(this->tm.timeUp()) {
            return -1;
        }

        ++this->nodes;
        this->max_seldepth = std::max(distanceFromRoot, this->max_seldepth);

        int stand_pat = this->board.getEvalScore();
        if(stand_pat >= beta)
            return beta;
        if(alpha < stand_pat)
            alpha = stand_pat;
        if(depthLeft == 0)
            return stand_pat;

        std::vector<BoardMove> moves = MoveGen::moveGenerator(this->board);
        MovePicker movePicker(std::move(moves));
        movePicker.assignMoveScores(board);

        int score = MIN_ALPHA;
        while (movePicker.movesLeft()) {
            BoardMove move = movePicker.pickMove();
            if(!board.moveIsCapture(move))
                continue;
            board.makeMove(move);
            score = -1 * (quiesce(-1 * beta, -1 * alpha, depthLeft - 1, distanceFromRoot + 1));
            board.undoMove(); 

            if(score >= beta)
                return beta;
            if(score > alpha)
                alpha = score;
        }
        return alpha;

    }
    
    void Searcher::storeInTT(TTable::Entry entry, Node result, int distanceFromRoot) {
        int posIndex = TTable::table.getIndex(this->board.zobristKey);
        /* entries in the transposition table are overwritten under two conditions:
        1. The current search depth is greater than the entry's depth, meaning that a better
        search has been performed 
        2. The age of the current position is greater than the previous age. Previous move searches
        in hash are preserved in the table since there can be repeated boards, but replacing entries
        with moves from more modern roots is better
        */
        if ( (distanceFromRoot >= entry.depth || this->board.fiftyMoveRule >= entry.age)
            && result.move != BoardMove()) {
                entry.key = static_cast<uint16_t>(this->board.zobristKey);
                entry.age = this->board.fiftyMoveRule;
                entry.depth = distanceFromRoot;
                entry.move = result.move;
                TTable::table.storeEntry(posIndex, entry);
        }
    }

    void Searcher::outputUciInfo(Info searchResult) {
        std::cout << "info depth " << searchResult.depth << ' ';
        std::cout << "seldepth " << searchResult.seldepth << ' ';
        std::cout << "nodes " << searchResult.nodes << ' ';

        // time is output in milliseconds per the UCI protocol
        std::cout << "time " << searchResult.timeElapsed << ' ';
        if (searchResult.timeElapsed > 0) { // prevents divide by 0
            std::cout << "nps " << searchResult.nodes * 1000 / searchResult.timeElapsed  << ' ';
        }

        std::cout << "pv " << searchResult.move << ' ';
        
        if (searchResult.mateIn == Search::NO_MATE) {
            std::cout << "score cp " << searchResult.eval << ' ';
        } else {
            int colorToMate = searchResult.eval < 0 ? -1 : 1;
            std::cout << "score mate " << (searchResult.mateIn + colorToMate) / 2 << ' '; // convert plies to moves
        }
        std::cout << "hashfull " << TTable::table.hashFull() << ' ';
        std::cout << std::endl;

    }

} // namespace Search