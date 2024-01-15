#include <cmath>
#include <iostream>

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

    // piece square tables
    tune_t op, eg;
    for (int i = 0; i < NUM_PIECES; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            op = Eval::tablesOp[i][j] - Eval::pieceValsOp[i];
            eg = Eval::tablesEg[i][j] - Eval::pieceValsEg[i];
            params.push_back(pair_t{op, eg});
        }
    }

    // intrinsic piece values
    for (int i = 0; i < NUM_PIECES; ++i) {
        op = Eval::pieceValsOp[i];
        eg = Eval::pieceValsEg[i];
        params.push_back(pair_t{op, eg});
    }

    // passed pawns
    for (int i = 0; i < NUM_FILES; ++i) {
        op = Eval::passedPawnOp[i];
        eg = Eval::passedPawnEg[i];
        params.push_back(pair_t{op, eg});
    }

    return params;
}

EvalResult BlockyEval::get_fen_eval_result(const std::string& fen) {
    const Board board(fen);
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
    std::cout << "PieceValsOp: \n{";
    printArr(parameters, PIECE_VALS_OFFSET, 0, NUM_PIECES);

    std::cout << "PieceValsEg: \n{";
    printArr(parameters, PIECE_VALS_OFFSET, 1, NUM_PIECES);

    std::cout << "PassedPawnsOp: \n{";
    printArr(parameters, PASSED_PAWNS_OFFSET, 0, NUM_FILES);

    std::cout << "PassedPawnsEg: \n{";
    printArr(parameters, PASSED_PAWNS_OFFSET, 1, NUM_FILES);

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

void printArr(const parameters_t& parameters, int offset, int index, int size) {
    for (int i = 0; i < size; ++i) {
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