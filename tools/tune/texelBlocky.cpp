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

void BlockyEval::pushEntry(parameters_t& parameters, Eval::S entry, const Eval::S adjustVal) {
    entry -= adjustVal;
    tune_t op = entry.opScore;
    tune_t eg = entry.egScore;
    parameters.push_back(pair_t{op, eg});
}

EvalResult BlockyEval::get_fen_eval_result(const std::string& fen) {
    Board board(fen);
    EvalResult result;
    const auto isWhitePiece = [](pieceTypes piece) {
        return piece >= WKing && piece <= WPawn;
    };

    int colorlessPiece, pieceOffset;
    int passedPawnFlag;

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

        const int colorlessPiece = piece % 6;
        const int pieceOffset = colorlessPiece * BOARD_SIZE;

        // adjust coefficients
        passedPawnFlag = 0;
        if (colorlessPiece == WPawn) {
            if (piece == WPawn) {
                passedPawnFlag = static_cast<int>(Eval::isPassedPawn(i, board.pieceSets[BPawn], true));
            } else {
                passedPawnFlag = -static_cast<int>(Eval::isPassedPawn(i, board.pieceSets[WPawn], false));
            }
        }

        // white and black pieces use different eval indices in piece square tables
        if (isWhitePiece(piece)) {
            result.coefficients[offsets["PSQT"] + pieceOffset + i]++;
            result.coefficients[offsets["pieceVals"] + colorlessPiece]++;
            result.coefficients[offsets["passedPawns"] + getRank(i)] += passedPawnFlag;
        } else {
            result.coefficients[offsets["PSQT"] + pieceOffset + (i ^ 56)]--;
            result.coefficients[offsets["pieceVals"] + colorlessPiece]--;
            result.coefficients[offsets["passedPawns"] + (getRank(i) ^ 7)] += passedPawnFlag;
        }
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