#include <iostream>

#include "texelBlocky.hpp"
#include "texel-tuner/src/base.h"
#include "../../src/board.hpp"
#include "../../src/eval.hpp"
#include "../../src/types.hpp"

namespace Blocky {

parameters_t BlockyEval::get_initial_parameters() {
    Eval::init();
    parameters_t params;
    tune_t op, eg;
    for (int i = 0; i < NUM_PIECE_TYPES / 2; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            op = Eval::tablesOp[i][j];
            eg = Eval::tablesEg[i][j];
            params.push_back(pair_t{op, eg});
        }
    }
    return params;
}

EvalResult BlockyEval::get_fen_eval_result(const std::string& fen) {
    Board board(fen);
    EvalResult result;
    auto isWhitePiece = [](pieceTypes piece) {
        return piece >= WKing && piece <= WPawn;
    };
    int pieceIndex, squareIndex;

    // coefficients are zero-initialized
    result.coefficients.resize(NUM_PIECE_TYPES / 2 * BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        pieceTypes piece = board.getPiece(i);
        // only squares with pieces shift the coefficients from zero
        if (piece != EmptyPiece) {
            continue;
        }
        // white and black pieces use different eval indices
        pieceIndex = (piece / 2) * BOARD_SIZE; 
        if (isWhitePiece(piece)) {
            result.coefficients[pieceIndex + i]++;
        } else {
            result.coefficients[pieceIndex + (i ^ 56)]--;
        }
    }
    result.score = board.evaluate();
    return result;
}

void BlockyEval::print_parameters(const parameters_t& parameters) {
    std::cout << "Midgame Tables: \n";
    printTable(parameters, 0);

    std::cout << "Endgame Tables: \n";
    printTable(parameters, 1);
}

void printTable(const parameters_t& parameters, int index) {
    for (int i = 0; i < NUM_PIECE_TYPES / 2; ++i) {
        std::cout << "Table " << pieceToChar.at(pieceTypes(i)) << ": \n";
        std::cout << "[";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            std::cout << parameters[i * BOARD_SIZE + j][index] << ", ";
            if (j % 8 == 7) {std::cout << "\n";}
        }
        std::cout << "]\n\n";
    }
}

} // namespace Blocky