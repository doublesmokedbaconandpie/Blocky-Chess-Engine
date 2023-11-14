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
    for(int i = 1; i <= this->depth_limit; i++) {
        result.eval = this->search<ROOT>(MIN_ALPHA, MAX_BETA, i, 0);
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

        // compute mate-in
        if (abs(result.eval) > MAX_BETA - 100) {
            int playerMating = result.eval < 0 ? -1 : 1;
            result.mateIn = playerMating * (MAX_BETA - abs(result.eval)); // convert eval to ply
            result.mateIn = (result.mateIn + playerMating) / 2; // convert ply to moves
        }

        if (this->printInfo) {
            this->outputUciInfo(result);
        }
        
        if (this->tm.timeUp()) {
            break;
        }
    }

    return result;
}

template <NodeTypes NODE>
int Searcher::search(int alpha, int beta, int depth, int distanceFromRoot) {

    // time up
    int score = 0;
    if(this->tm.timeUp()) {
        return score;
    }

    ++this->nodes;
    this->max_seldepth = std::max(distanceFromRoot, this->max_seldepth);

    // fifty move rule
    if (this->board.fiftyMoveRule == 100) {
        return score;
    }
    // three-fold repetition
    std::vector<uint64_t> currKeyHistory = this->board.zobristKeyHistory;
    std::sort(currKeyHistory.begin(), currKeyHistory.end());
    auto lBound = std::lower_bound(currKeyHistory.begin(), currKeyHistory.end(), this->board.zobristKey);
    auto rBound = std::upper_bound(currKeyHistory.begin(), currKeyHistory.end(), this->board.zobristKey);
    if (distance(lBound, rBound) == 3) {
        return score;
    }
    // max depth reached
    if (depth == 0) {
        return quiesce(alpha, beta, 5, distanceFromRoot);
    }
    // checkmate or stalemate
    std::vector<BoardMove> moves = MoveGen::moveGenerator(this->board);
    if (moves.size() == 0) {
        if (currKingInAttack(board.pieceSets, board.isWhiteTurn)) {
            score = MIN_ALPHA + distanceFromRoot;
        }
        return score;
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
    MoveOrder::MovePicker movePicker(std::move(moves));
    movePicker.assignMoveScores(board, PVNode);

    // start search through moves
    int bestscore = MIN_ALPHA;
    BoardMove bestMove;
    while (movePicker.movesLeft()) {
        BoardMove move = movePicker.pickMove();
        board.makeMove(move);
        score = -search<PV>(-beta, -alpha, depth - 1, distanceFromRoot + 1);
        board.undoMove(); 

        // don't update best move if time is up
        if (this->tm.timeUp()) {
            return score;
        }
        
        // prune if a move is too good; opponent side will avoid playing into this node
        if (score >= beta) {
            score = beta;
            break;
        }
        // fail-soft stabilizes the search and allows for returned values outside the alpha-beta bounds
        if (score > bestscore) {
            bestscore = score;
            bestMove = move;
            if constexpr (NODE == ROOT) {
                this->finalMove = bestMove;
            }
            if (score > alpha) {
                alpha = score;
            }
        }
    }
    // A search for this depth is complete with a best move, so it can be stored in the transposition table
    this->storeInTT(entry, bestMove, depth);
    return score;
}

int Searcher::quiesce(int alpha, int beta, int depth, int distanceFromRoot) {
    if(this->tm.timeUp()) {
        return 0;
    }

    ++this->nodes;
    this->max_seldepth = std::max(distanceFromRoot, this->max_seldepth);

    int stand_pat = this->board.evaluate();
    if(stand_pat >= beta)
        return beta;
    if(alpha < stand_pat)
        alpha = stand_pat;
    if(depth == 0)
        return stand_pat;

    std::vector<BoardMove> moves = MoveGen::moveGenerator(this->board);
    MoveOrder::MovePicker movePicker(std::move(moves));
    movePicker.assignMoveScores(board);

    int score = MIN_ALPHA;
    while (movePicker.movesLeft()) {
        BoardMove move = movePicker.pickMove();
        if(!board.moveIsCapture(move))
            continue;
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

void Searcher::storeInTT(TTable::Entry entry, BoardMove move, int depth) {
    int posIndex = TTable::table.getIndex(this->board.zobristKey);
    /* entries in the transposition table are overwritten under two conditions:
    1. The current search depth is greater than the entry's depth, meaning that a better
    search has been performed 
    2. The age of the current position is greater than the previous age. Previous move searches
    in hash are preserved in the table since there can be repeated boards, but replacing entries
    with moves from more modern roots is better
    */
    if ( (depth >= entry.depth || this->board.fiftyMoveRule >= entry.age) 
        && move != BoardMove()) {
            entry.key = static_cast<uint16_t>(this->board.zobristKey);
            entry.age = this->board.fiftyMoveRule;
            entry.depth = depth;
            entry.move = move;
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
        int playerMating = searchResult.eval < 0 ? -1 : 1;
        std::cout << "score mate " << searchResult.mateIn << ' ';
    }
    std::cout << "hashfull " << TTable::table.hashFull() << ' ';
    std::cout << std::endl;

}

} // namespace Search