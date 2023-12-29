#include <algorithm>
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

    // perform iterative deepening
    for (int i = 1; i <= this->depth_limit; i++) {
        const int score = this->search<ROOT>(MIN_ALPHA, MAX_BETA, i, 0);
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
        if (!tm.hardTimeUp()) {
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
        
        // only break out of search early for optimistic time used
        if (this->tm.softTimeUp()) {
            break;
        }
    }

    return result;
}

template <NodeTypes NODE>
int Searcher::search(int alpha, int beta, int depth, int distanceFromRoot) {
    constexpr bool ISROOT = NODE == ROOT;
    constexpr bool ISPV = NODE == ROOT || NODE == PV;
    constexpr bool ISNMP = NODE == NMP;
    const int oldAlpha = alpha;

    // time up
    int score = 0;
    if(this->tm.hardTimeUp()) {
        return score;
    }

    ++this->nodes;
    this->max_seldepth = std::max(distanceFromRoot, this->max_seldepth);

    // fifty move rule
    if (this->board.fiftyMoveRule >= 100) {
        return score;
    }
    // three-fold repetition
    const int occurrences = std::count(this->board.zobristKeyHistory.begin(), this->board.zobristKeyHistory.end(), this->board.zobristKey);
    if (occurrences >= 3) {
        return score;
    }
    // max depth reached
    if (depth <= 0) {
        return quiesce(alpha, beta, 6, distanceFromRoot);
    }

    /************
     * Probe Tranposition Table
    *************/
    BoardMove TTMove;
    TTable::Entry entry;
    if (TTable::table.entryExists(this->board.zobristKey)) {
        entry = TTable::table.getEntry(this->board.zobristKey);

        if (!ISPV && entry.depth >= depth) {
            if (entry.flag == EvalType::EXACT
                || (entry.flag == EvalType::UPPER && entry.eval <= alpha)
                || (entry.flag == EvalType::LOWER && entry.eval >= beta)) {
                return entry.eval;
            }
        }

        TTMove = entry.move;
    }

    /************
     * Null Move Pruning
    *************/
    if (!ISNMP && depth >= 2 && !currKingInAttack(board.pieceSets, board.isWhiteTurn)) {
        board.makeNullMove();
        int nullMoveScore = -search<NMP>(-beta, -beta + 1, depth - 3, distanceFromRoot + 1);
        board.unmakeNullMove();
        if (nullMoveScore >= beta) {
            return nullMoveScore;
        }
    }

    // init movePicker
    MoveOrder::MovePicker movePicker(board, MoveOrder::All, TTMove);

    // start search through moves
    int bestscore = MIN_ALPHA;
    BoardMove bestMove, move;
    bool doFullNullSearch, doPVS;
    while (movePicker.movesLeft(board)) {
        move = movePicker.pickMove();
        board.makeMove(move);
        /*************
         * Late Move Reductions (LMR):
         * Search moves that are likely to be less good to lower depths with null bounds
         * Researches will happen with LMR fails
        **************/
        if (!movePicker.stagesLeft() // don't reduce captures
            && movePicker.getMovesPicked() >= 4 
            && depth >= 3
            && !currKingInAttack(this->board.pieceSets, board.isWhiteTurn)) {
            
            int reductionDepth = depth - 2;
            score = -search<NOTPV>(-alpha - 1, -alpha, reductionDepth, distanceFromRoot + 1);
            doFullNullSearch = score > alpha;
        } else {
            doFullNullSearch = !ISPV || movePicker.getMovesPicked() > 1;
        }

        if (doFullNullSearch) {
            score = -search<NOTPV>(-alpha - 1, -alpha, depth - 1, distanceFromRoot + 1);
        }
        /*************
         * Principle Variation Search (PVS):
         * Search with full bounds with null bounds fail
        **************/
        doPVS = ISPV && ((score > alpha && score < beta) || movePicker.getMovesPicked() == 1);
        if (doPVS) {
            score = -search<PV>(-beta, -alpha, depth - 1, distanceFromRoot + 1);
        }
        board.undoMove(); 

        // don't update best move if time is up
        if (this->tm.hardTimeUp()) {
            return bestscore;
        }
        
        // prune if a move is too good; opponent side will avoid playing into this node
        if (score >= beta) {
            bestscore = score;
            break;
        }
        // fail-soft stabilizes the search and allows for returned values outside the alpha-beta bounds
        if (score > bestscore) {
            bestscore = score;
            bestMove = move;
            if (ISROOT) {
                this->finalMove = bestMove;
            }
            if (score > alpha) {
                alpha = score;
            }
        }
    }
    // checkmate or stalemate
    if (movePicker.getMovesPicked() == 0) {
        if (currKingInAttack(board.pieceSets, board.isWhiteTurn)) {
            score = MIN_ALPHA + distanceFromRoot;
        }
        return score;
    }

    // A search for this depth is complete with a best move, so it can be stored in the transposition table
    entry.flag = (bestscore >= beta) ? EvalType::LOWER : (alpha == oldAlpha) ? EvalType::UPPER : EvalType::EXACT;
    this->storeInTT(entry, bestscore, bestMove, depth);
    return bestscore;
}

int Searcher::quiesce(int alpha, int beta, int depth, int distanceFromRoot) {
    if(this->tm.hardTimeUp()) {
        return 0;
    }

    ++this->nodes;
    this->max_seldepth = std::max(distanceFromRoot, this->max_seldepth);

    const int stand_pat = this->board.evaluate();
    if(stand_pat >= beta)
        return beta;
    if(alpha < stand_pat)
        alpha = stand_pat;
    if(depth == 0)
        return stand_pat;

    MoveOrder::MovePicker movePicker(board, MoveOrder::Captures);
    int score = MIN_ALPHA;
    while (movePicker.movesLeft(board)) {
        BoardMove move = movePicker.pickMove();
        board.makeMove(move);
        score = -quiesce(-beta, -alpha, depth - 1, distanceFromRoot + 1);
        board.undoMove(); 

        if(score >= beta)
            return beta;
        if(score > alpha)
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
    if ( (depth >= entry.depth || this->board.age >= entry.age)
        && move) {
            entry.key = this->board.zobristKey;
            entry.age = this->board.age;
            entry.depth = depth;
            entry.eval = eval;
            entry.move = move;
            TTable::table.storeEntry(entry.key, entry);
    }
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
    
    if (searchResult.mateIn == Search::NO_MATE) {
        std::cout << "score cp " << searchResult.eval << ' ';
    } else {
        std::cout << "score mate " << searchResult.mateIn << ' ';
    }
    std::cout << "hashfull " << TTable::table.hashFull() << ' ';
    std::cout << std::endl;

}

} // namespace Search
