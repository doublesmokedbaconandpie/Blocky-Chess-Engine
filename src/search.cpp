#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "search.hpp"
#include "ttable.hpp"
#include "moveOrder.hpp"
#include "eval.hpp"
#include "moveGen.hpp"
#include "board.hpp"
#include "timeman.hpp"

namespace Search {

Info Searcher::startThinking() {
    Info result;

    // stack needs to label distances from root
    for (size_t i = 0; i < this->stack.size(); ++i) {
        this->stack[i].distanceFromRoot = i;
    }

    // perform iterative deepening
    int prevEval = NO_SCORE;
    for (int i = 1; i <= this->depth_limit; i++) {
        const int score = this->aspiration(i, prevEval);
        prevEval = score;
        result.move = this->finalMove;
        result.nodes = this->nodes;
        result.timeElapsed = this->tm.getTimeElapsed();

        // if it's not possible to search deeper, stop searching 
        if (this->max_seldepth < i) {
            break;
        } else {
            result.depth = i;
            result.seldepth = this->max_seldepth;
        }

        // only update eval for completed searches
        if (!this->stopSearching()) {
            result.eval = score;
        }

        // compute mate-in
        if (abs(result.eval) > MAX_BETA - 100) {
            int playerMating = result.eval < 0 ? -1 : 1;
            result.mateIn = playerMating * (MAX_BETA - abs(result.eval)); // convert eval to ply
            result.mateIn = (result.mateIn + playerMating) / 2; // convert ply to moves
        }

        if (this->printInfo) {
            this->outputUciInfo(result);
        }
        
        // break out of search early for optimistic time used; this also applies to hard time up
        if (this->tm.softTimeUp()) {
            break;
        }
    }

    return result;
}

int Searcher::aspiration(int depth, int prevEval) {
    int delta = 50;
    int alpha, beta;

    // don't use aspiration bounds for low search depths as scores at low depths are less stable
    if (depth <= 6) {
        alpha = MIN_ALPHA;
        beta = MAX_BETA;
    } else {
        alpha = prevEval - delta;
        beta = prevEval + delta;
    }

    // search until an exact score has been found with the aspiration bounds or search has been stopped
    int result;
    while (true) {
        result = this->search<ROOT>(alpha, beta, depth, &this->stack[0]);

        if (this->stopSearching() || (alpha < result && result < beta)) {
            break;
        }

        alpha = std::max(alpha - delta, MIN_ALPHA);
        beta = std::min(beta + delta, MAX_BETA);
        delta *= 2;
    }
    return result;
}

template <NodeTypes NODE>
int Searcher::search(int alpha, int beta, int depth, StackEntry* ss) {
    constexpr bool ISROOT = NODE == ROOT;
    constexpr bool ISPV = NODE == ROOT || NODE == PV;
    constexpr bool ISNMP = NODE == NMP;
    const int oldAlpha = alpha;

    // time up
    if (this->stopSearching()) {
        return NO_SCORE;
    }

    ++this->nodes;
    this->max_seldepth = std::max(ss->distanceFromRoot, this->max_seldepth);

    // fifty move rule
    if (this->board.fiftyMoveRule >= 100) {
        return DRAW_SCORE;
    }
    // three-fold repetition
    const int occurrences = std::count(this->board.zobristKeyHistory.begin(), this->board.zobristKeyHistory.end(), this->board.zobristKey);
    if (occurrences >= 3) {
        return DRAW_SCORE;
    }
    // max depth reached
    if (depth <= 0) {
        return quiesce(alpha, beta, ss);
    }

    /************
     * Probe Tranposition Table
    *************/
    BoardMove TTMove;
    TTable::Entry entry;
    if (TTable::Table.entryExists(this->board.zobristKey)) {
        entry = TTable::Table.getEntry(this->board.zobristKey);

        if (!ISPV && entry.depth >= depth) {
            if (entry.flag == EvalType::EXACT
                || (entry.flag == EvalType::UPPER && entry.eval <= alpha)
                || (entry.flag == EvalType::LOWER && entry.eval >= beta)) {
                return entry.eval;
            }
        }

        TTMove = entry.move;
    }

    const bool inCheck = currKingInAttack(this->board.pieceSets, this->board.isWhiteTurn);
    /************
     * Null Move Pruning
     * Give the opponent a free move and see if our position is still too good after that; if so, prune
     * This assumes that the opponent cannot improve their position by much given our reduced depth
    *************/
    if (!ISNMP && !inCheck && depth >= 2) {
        board.makeNullMove();
        int nullMoveScore = -search<NMP>(-beta, -beta + 1, depth - 3, ss + 1);
        board.unmakeNullMove();
        if (nullMoveScore >= beta) {
            return nullMoveScore;
        }
    }

    // init movePicker
    MoveOrder::MovePicker movePicker(board, MoveOrder::All, TTMove, ss->killerMove);

    // start search through moves
    int score, bestscore = MIN_ALPHA;
    BoardMove bestMove;
    bool doFullNullSearch, doPVS;
    while (movePicker.movesLeft(board)) {
        const BoardMove move = movePicker.pickMove();
        board.makeMove(move);
        const bool moveGivesCheck = currKingInAttack(this->board.pieceSets, this->board.isWhiteTurn);
        const bool quietMove = !movePicker.stagesLeft();

        /*************
         * Extensions:
        **************/
        int extensions = 0;
        extensions += static_cast<int>(moveGivesCheck);
        int newDepth = depth + extensions;

        /*************
         * Late Move Reductions (LMR):
         * Search moves that are likely to be less good to lower depths with null bounds
         * Researches will happen with LMR fails
        **************/
        if (quietMove
            && movePicker.getMovesPicked() >= 4 
            && depth >= 3
            && !moveGivesCheck) {
            
            score = -search<NOTPV>(-alpha - 1, -alpha, newDepth - 2, ss + 1);
            doFullNullSearch = score > alpha;
        } else {
            doFullNullSearch = !ISPV || movePicker.getMovesPicked() > 1;
        }

        if (doFullNullSearch) {
            score = -search<NOTPV>(-alpha - 1, -alpha, newDepth - 1, ss + 1);
        }
        /*************
         * Principle Variation Search (PVS):
         * Search with full bounds with null bounds fail
        **************/
        doPVS = ISPV && ((score > alpha && score < beta) || movePicker.getMovesPicked() == 1);
        if (doPVS) {
            score = -search<PV>(-beta, -alpha, newDepth - 1, ss + 1);
        }
        board.undoMove(); 

        // don't update best move if time is up
        if (this->stopSearching()) {
            return bestscore;
        }
        
        // fail-soft stabilizes the search and allows for returned values outside the alpha-beta bounds
        if (score > bestscore) {
            bestscore = score;
            bestMove = move;
            if (ISROOT) {
                this->finalMove = bestMove;
            }

            // update alpha if we have proven that we can guarantee that lower bound
            if (score > alpha) {
                alpha = score;

                // prune if a move is too good, opponent will avoid playing into this node
                if (score >= beta) {
                    MoveOrder::History[move.sqr1()][move.sqr2()] += depth * (depth - 1);
                    if (quietMove) {
                        ss->killerMove = move;
                    }
                    break;
                }
            }
        }
    }
    // checkmate or stalemate
    if (movePicker.getMovesPicked() == 0) {
        if (inCheck) {
            return MIN_ALPHA + ss->distanceFromRoot;
        }
        return DRAW_SCORE;
    }

    // store results with best moves in transposition table
    if (bestMove) {
        entry.flag = (bestscore >= beta) ? EvalType::LOWER : (alpha == oldAlpha) ? EvalType::UPPER : EvalType::EXACT;
        this->storeInTT(entry, bestscore, bestMove, depth);
    }
    return bestscore;
}

int Searcher::quiesce(int alpha, int beta, StackEntry* ss) {
    if (this->stopSearching()) {
        return NO_SCORE;
    }

    ++this->nodes;
    this->max_seldepth = std::max(ss->distanceFromRoot, this->max_seldepth);

    const int stand_pat = this->board.evaluate();
    if (stand_pat >= beta)
        return beta;
    if (alpha < stand_pat)
        alpha = stand_pat;

    MoveOrder::MovePicker movePicker(board, MoveOrder::Captures);
    int score = MIN_ALPHA;
    while (movePicker.movesLeft(board)) {
        BoardMove move = movePicker.pickMove();
        board.makeMove(move);
        score = -quiesce(-beta, -alpha, ss + 1);
        board.undoMove(); 

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;

}

void Searcher::storeInTT(TTable::Entry entry, int eval, BoardMove move, int depth) const {
    /* entries in the transposition table are overwritten under two conditions:
    1. The current search depth is greater than the entry's depth, meaning that a better
    search has been performed 
    2. The age of the current position is greater than the previous age. Previous move searches
    in hash are preserved in the table since there can be repeated boards, but replacing entries
    with moves from more modern roots is better
    */
    if (depth >= entry.depth || this->board.age >= entry.age) {
            entry.key = this->board.zobristKey;
            entry.age = this->board.age;
            entry.depth = depth;
            entry.eval = eval;
            entry.move = move;
            TTable::Table.storeEntry(entry.key, entry);
    }
}

bool Searcher::stopSearching() {
    // only check system time every 1024 nodes for performance
    if (this->nodes % 1024 == 0 && !this->stopSearchFlag) {
        this->stopSearchFlag = this->tm.hardTimeUp();
    }
    return this->stopSearchFlag;
}

void Searcher::outputUciInfo(Info searchResult) const {
    std::cout << "info depth " << searchResult.depth << ' ';
    std::cout << "seldepth " << searchResult.seldepth << ' ';
    std::cout << "nodes " << searchResult.nodes << ' ';

    // time is output in milliseconds per the UCI protocol
    std::cout << "time " << searchResult.timeElapsed << ' ';
    if (searchResult.timeElapsed > 0) { // prevents divide by 0
        std::cout << "nps " << searchResult.nodes * 1000 / searchResult.timeElapsed  << ' ';
    }

    std::cout << "pv " << searchResult.move << ' ';
    
    if (searchResult.mateIn == NO_SCORE) {
        std::cout << "score cp " << searchResult.eval << ' ';
    } else {
        std::cout << "score mate " << searchResult.mateIn << ' ';
    }
    std::cout << "hashfull " << TTable::Table.hashFull() << ' ';
    std::cout << std::endl;

}

} // namespace Search
