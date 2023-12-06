#include <cmath>
#include <iostream>

#include "texelBlocky.hpp"
#include "texel-tuner/src/base.h"
#include "../../src/board.hpp"
#include "../../src/eval.hpp"
#include "../../src/types.hpp"

namespace Blocky {

constexpr int PSQTOffset = NUM_PIECES * BOARD_SIZE;

parameters_t BlockyEval::get_initial_parameters() {
    parameters_t params;

    // piece square tables
    tune_t op, eg;
    for (int i = 0; i < NUM_PIECES; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            op = Eval::tablesOp[i][j];
            eg = Eval::tablesEg[i][j];
            params.push_back(pair_t{op, eg});
        }
    }

    // intrinsic piece values
    for (int i = 0; i < NUM_PIECES; ++i) {
        op = Eval::pieceValsOp[i];
        eg = Eval::pieceValsEg[i];
        params.push_back(pair_t{op, eg});
    }

    return params;
}

EvalResult BlockyEval::get_fen_eval_result(const std::string& fen) {
    const Board board(fen);
    EvalResult result;
    auto isWhitePiece = [](pieceTypes piece) {
        return piece >= WKing && piece <= WPawn;
    };
    int arrSize;

    /************
     * Initialize coefficients to zero-weights, which should fit most of them
    *************/
    arrSize = PSQTOffset + NUM_PIECES;
    result.coefficients.resize(arrSize);

    /************
     * Assign non-zero linear weights to coefficients for the given position
    *************/
    for (int i = 0; i < BOARD_SIZE; ++i) {
        pieceTypes piece = board.getPiece(i);
        // empty squares don't affect evaluation
        if (piece == EmptyPiece) {
            continue;
        }

        // white and black pieces use different eval indices in piece square tables
        int colorlessPiece = piece % 6;
        int pieceOffset = colorlessPiece * BOARD_SIZE;
        if (isWhitePiece(piece)) {
            result.coefficients[pieceOffset + i]++;
            result.coefficients[PSQTOffset + colorlessPiece]++;
        } else {
            result.coefficients[pieceOffset + (i ^ 56)]--;
            result.coefficients[PSQTOffset + colorlessPiece]--;
        }
    }

    result.score = board.evaluate();
    return result;
}

void BlockyEval::print_parameters(const parameters_t& parameters) {
    std::cout << "Opening Pieces: {\n";
    printArr(parameters, PSQTOffset, 0);

    std::cout << "Ending Pieces: {\n";
    printArr(parameters, PSQTOffset, 1);

    std::cout << "Opening PSQT: \n";
    printPSQT(parameters, 0);

    std::cout << "Endgame PSQT: \n";
    printPSQT(parameters, 1);
}

void printPSQT(const parameters_t& parameters, int index) {
    for (int i = 0; i < NUM_PIECES; ++i) {
        std::cout << "Table " << pieceToChar.at(pieceTypes(i)) << ": {\n";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            std::cout << std::round(parameters[i * BOARD_SIZE + j][index]) << ", ";
            if (j % 8 == 7) {std::cout << "\n";}
        }
        std::cout << "};\n\n";
    }
}

void printArr(const parameters_t& parameters, int offset, int index) {
    for (int i = 0; i < NUM_PIECES; ++i) {
        std::cout << std::round(parameters[offset + i][index]) << ", ";
    }
    std::cout << "};\n\n";
}

void printCoeff(const parameters_t& parameters, int index) {
    for (const auto i: parameters) {
        std::cout << i[index] << ", ";
    }
    std::cout << "};\n\n";
}

} // namespace Blocky