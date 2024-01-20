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

constexpr int PSQT_SIZE = NUM_PIECES * BOARD_SIZE;
constexpr int PIECE_VALS_SIZE = NUM_PIECES;
constexpr int PASSED_PAWNS_SIZE = NUM_RANKS;
constexpr int TOTAL_SIZE = PSQT_SIZE + PIECE_VALS_SIZE + PASSED_PAWNS_SIZE;

constexpr int PSQT_OFFSET = 0;
constexpr int PIECE_VALS_OFFSET = PSQT_OFFSET + PSQT_SIZE;
constexpr int PASSED_PAWNS_OFFSET = PIECE_VALS_OFFSET + PIECE_VALS_SIZE;
constexpr int MOBILITY_OFFSET = PASSED_PAWNS_OFFSET + PASSED_PAWNS_SIZE;

parameters_t BlockyEval::get_initial_parameters() {
    parameters_t params;

    // push piece square tables
    for (int i = 0; i < NUM_PIECES; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            pushEntry(params, Eval::PSQT[i][j] - Eval::pieceVals[i]);
        }
    }
    pushTable(params, Eval::pieceVals);
    pushTable(params, Eval::passedPawn);
    return params;
}

template<typename T>
void BlockyEval::pushTable(parameters_t& parameters, T& table) {
    for (const auto& entry: table) {
        pushEntry(parameters, entry);
    }
}

void BlockyEval::pushEntry(parameters_t& parameters, Eval::S entry) {
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
    result.coefficients.resize(TOTAL_SIZE);

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
            result.coefficients[PSQT_OFFSET + pieceOffset + i]++;
            result.coefficients[PIECE_VALS_OFFSET + colorlessPiece]++;
            result.coefficients[PASSED_PAWNS_OFFSET + getRank(i)] += passedPawnFlag;
        } else {
            result.coefficients[pieceOffset + (i ^ 56)]--;
            result.coefficients[PIECE_VALS_OFFSET + colorlessPiece]--;
            result.coefficients[PASSED_PAWNS_OFFSET + (getRank(i) ^ 7)] += passedPawnFlag;
        }
    }

    result.score = board.evaluate();
    return result;
}

void BlockyEval::print_parameters(const parameters_t& parameters) {
    std::cout << "PassedPawns: \n{";
    printArr(parameters, PASSED_PAWNS_OFFSET, PASSED_PAWNS_SIZE);

    std::cout << "PieceVals: \n{";
    printArr(parameters, PIECE_VALS_OFFSET, PIECE_VALS_SIZE);

    std::cout << "PSQT: \n";
    printPSQT(parameters);
}

void printPSQT(const parameters_t& parameters) {
    for (int i = 0; i < NUM_PIECES; ++i) {
        std::cout << "Table " << pieceToChar.at(pieceTypes(i)) << ": {\n";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            std::cout << toStr(parameters[i * BOARD_SIZE + j], 4) << ", ";

            if (j % 8 == 7) {
                std::cout << "\n";
            }
        }
        std::cout << "};\n\n";
    }
}

void printArr(const parameters_t& parameters, int offset, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << toStr(parameters[offset + i], 3) << ", ";
    }
    std::cout << "};\n\n";
}

void printCoeff(const parameters_t& parameters) {
    for (const auto i: parameters) {
        std::cout << toStr(i, 4) << ", ";
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