#include <array>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "texelBlocky.hpp"
#include "texel-tuner/src/base.h"
#include "../../src/board.hpp"
#include "../../src/eval.hpp"
#include "../../src/bitboard.hpp"
#include "../../src/types.hpp"

namespace Blocky {

std::map<std::string, int> BlockyEval::offsets;
std::map<std::string, int> BlockyEval::sizes;
std::vector<std::string> BlockyEval::tablesInOrder;
int BlockyEval::totalSize;

parameters_t BlockyEval::get_initial_parameters() {
    parameters_t params;

    // piece square tables
    offsets["PSQT"] = 0;
    pushTable(params, "King PSQT", Eval::PSQT[WKing], Eval::pieceVals[WKing]);
    pushTable(params, "Queen PSQT", Eval::PSQT[WQueen], Eval::pieceVals[WQueen]);
    pushTable(params, "Bishop PSQT", Eval::PSQT[WBishop], Eval::pieceVals[WBishop]);
    pushTable(params, "Knight PSQT", Eval::PSQT[WKnight], Eval::pieceVals[WKnight]);
    pushTable(params, "Rook PSQT", Eval::PSQT[WRook], Eval::pieceVals[WRook]);
    pushTable(params, "Pawn PSQT", Eval::PSQT[WPawn], Eval::pieceVals[WPawn]);

    // misc piece arrays
    pushTable(params, "pieceVals", Eval::pieceVals);
    pushTable(params, "passedPawns", Eval::passedPawn);

    // misc evaluation terms
    pushSingleTerm(params, "doubledPawns", Eval::doubledPawns);
    pushSingleTerm(params, "chainedPawns", Eval::chainedPawns);

    totalSize = params.size();
    return params;
}

template<size_t N>
void BlockyEval::pushTable(parameters_t& parameters, std::string tableName, 
                            const std::array<Eval::S, N>& table, const Eval::S adjustVal) {

    // keep track of the table
    assert(offsets.find(tableName) == offsets.end());
    assert(sizes.find(tableName) == sizes.end());
    tablesInOrder.push_back(tableName);
    offsets[tableName] = parameters.size();
    sizes[tableName] = table.size();

    // push the table
    for (const auto& entry: table) {
        pushEntry(parameters, entry, adjustVal);
    }
}

void BlockyEval::pushSingleTerm(parameters_t& parameters, std::string termName, const Eval::S& term) {
    // keep track of the term
    assert(offsets.find(termName) == offsets.end());
    assert(sizes.find(termName) == sizes.end());
    tablesInOrder.push_back(termName);
    offsets[termName] = parameters.size();
    sizes[termName] = 1;

    // push the term
    pushEntry(parameters, term);
}

// helper to push back a single entry, regardless of table or evaluation single term
void BlockyEval::pushEntry(parameters_t& parameters, Eval::S entry, const Eval::S adjustVal) {
    entry -= adjustVal;
    tune_t op = entry.opScore;
    tune_t eg = entry.egScore;
    parameters.push_back(pair_t{op, eg});
}

EvalResult BlockyEval::get_fen_eval_result(const std::string& fen) {
    Board board(fen);
    EvalResult result;

    /************
     * Initialize coefficients to zero-weights, which should fit most of them
    *************/
    result.coefficients.resize(totalSize);

    /************
     * Assign non-zero linear weights to coefficients for the given position
    *************/
    for (int i = 0; i < BOARD_SIZE; ++i) {
        const pieceTypes piece = board.getPiece(i);
        // empty squares don't affect evaluation
        if (piece == EmptyPiece) {
            continue;
        }

        // helper information
        const bool isWhitePiece = piece >= WKing && piece <= WPawn;
        const int colorlessPiece = piece % 6;
        const int pieceOffset = colorlessPiece * BOARD_SIZE;

        // pawn terms
        int passedPawnFlag = 0, doubledPawnFlag = 0, chainedPawnFlag = 0;
        if (colorlessPiece == WPawn) {
            const uint64_t allyPawnSet = isWhitePiece ? board.pieceSets[WPawn] : board.pieceSets[BPawn];
            const uint64_t enemyPawnSet = isWhitePiece ? board.pieceSets[BPawn] : board.pieceSets[WPawn];
            const uint64_t doubledPawnSet = Eval::getDoubledPawnsMask(allyPawnSet, isWhitePiece);
            const uint64_t chainedPawnSet = Eval::getChainedPawnsMask(allyPawnSet, isWhitePiece);

            passedPawnFlag = static_cast<int>(Eval::isPassedPawn(i, enemyPawnSet, isWhitePiece));
            doubledPawnFlag = static_cast<int>(static_cast<bool>(doubledPawnSet & c_u64(1) << i));
            chainedPawnFlag = static_cast<int>(static_cast<bool>(chainedPawnSet & c_u64(1) << i));
        }

        // white and black pieces use different eval indices in piece square tables
        const int squareOffset = isWhitePiece ? i : i ^ 56;
        const int rankOffset = isWhitePiece ? getRank(i) : getRank(i) ^ 7;
        const int occurences = isWhitePiece ? 1 : -1;

        // add the coefficients
        result.coefficients[offsets["PSQT"] + pieceOffset + squareOffset] += occurences;
        result.coefficients[offsets["pieceVals"] + colorlessPiece] += occurences;
        result.coefficients[offsets["passedPawns"] + rankOffset] += passedPawnFlag * occurences;
        result.coefficients[offsets["doubledPawns"]] += doubledPawnFlag * occurences;
        result.coefficients[offsets["chainedPawns"]] += chainedPawnFlag * occurences;
    }

    result.score = board.evaluate();
    return result;
}

void BlockyEval::print_parameters(const parameters_t& parameters) {
    for (const auto tableName: tablesInOrder) {
        std::cout << tableName << ":\n";
        printArr(parameters, offsets[tableName], sizes[tableName]);
    }
}

void printArr(const parameters_t& parameters, int offset, int size) {
    // gets which width to print with; negatives require more space
    // -100 requires 4 spaces, other numbers require 3
    int width = 3;
    for (int i = 0; i < size; ++i) {
        if (parameters[offset + i][0] < -99 || parameters[offset + i][1] < -99) {
            width = 4;
            break;
        }
    }

    // actually print the parameters
    for (int i = 0; i < size; ++i) {
        std::cout << toStr(parameters[offset + i], width) << ", ";
        if (i % 8 == 7) {
            std::cout << '\n';
        }
    }
    std::cout << "};\n\n";
}

std::string toStr(pair_t val, int width) {
    std::ostringstream op, eg;
    op << std::setw(width) << std::round(val[0]);
    eg << std::setw(width) << std::round(val[1]);
    return "S(" + op.str() + "," + eg.str() + ")";
}

} // namespace Blocky