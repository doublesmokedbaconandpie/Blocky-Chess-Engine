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

constexpr int PIECE_VALS_OFFSET = NUM_PIECES * BOARD_SIZE;
constexpr int PASSED_PAWNS_OFFSET = PIECE_VALS_OFFSET + NUM_PIECES;

parameters_t BlockyEval::get_initial_parameters() {
    parameters_t params;
    tune_t op, eg;
    Eval::S entry;

    // piece square tables
    for (int i = 0; i < NUM_PIECES; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            entry = Eval::PSQT[i][j] - Eval::pieceVals[i];
            op = entry.opScore;
            eg = entry.egScore;
            params.push_back(pair_t{op, eg});
        }
    }

    // intrinsic piece values
    for (int i = 0; i < NUM_PIECES; ++i) {
        entry = Eval::pieceVals[i];
        op = entry.opScore;
        eg = entry.egScore;
        params.push_back(pair_t{op, eg});
    }

    // passed pawns
    for (int i = 0; i < NUM_FILES; ++i) {
        entry = Eval::passedPawn[i];
        op = entry.opScore;
        eg = entry.egScore;
        params.push_back(pair_t{op, eg});
    }

    return params;
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
    int arrSize = PASSED_PAWNS_OFFSET + NUM_FILES;
    result.coefficients.resize(arrSize);

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
                passedPawnFlag = static_cast<int>(isPassedPawn(i, board.pieceSets[BPawn], true));
            } else {
                passedPawnFlag = -static_cast<int>(isPassedPawn(i, board.pieceSets[WPawn], false));
            }
        }

        // white and black pieces use different eval indices in piece square tables
        if (isWhitePiece(piece)) {
            result.coefficients[pieceOffset + i]++;
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
    std::cout << "PieceVals: \n{";
    printArr(parameters, PIECE_VALS_OFFSET, NUM_PIECES);

    std::cout << "PassedPawns: \n{";
    printArr(parameters, PASSED_PAWNS_OFFSET, NUM_FILES);

    std::cout << "PSQT: \n";
    printPSQT(parameters);
}

void printPSQT(const parameters_t& parameters) {
    for (int i = 0; i < NUM_PIECES; ++i) {
        std::cout << "Table " << pieceToChar.at(pieceTypes(i)) << ": {\n";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            std::cout << toStr(parameters[i * BOARD_SIZE + j], 4) << ", ";
            if (j % 8 == 7) {std::cout << "\n";}
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