#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "attacks.hpp"
#include "board.hpp"
#include "eval.hpp"
#include "tune.hpp"

// tunes piece square tables using texel's tuning method

// parameters
const std::string data = "../data/positions.txt";
const std::string tmpFileDest = "../output/tmpParams.txt";
const std::string finalFileDest = "../output/finalParams.txt";
int adjustVal = 4; 
constexpr double errorUncertainty = 0.0001;
// K constant in sigmoid function to fit 100 centipawns = 0.7 score
// using equation f(s) = 1 / (e ^ (Ks))
constexpr double K = 0.003567;     

// global variables
std::vector<PositionData> dataVals;
double bestError;
int iterations;

int main() {
    // init
    Attacks::init();
    extractData();

    // variables 
    bestError = meanSquareError();
    double oldError;

    std::cout << "Original error: " << bestError << std::endl;
    // tune until local maximum found
    while (adjustVal) {
        std::cout << "Using adjust val: " << adjustVal << std::endl;
        while (true) {
            oldError = bestError;
            tuneTables(Eval::tablesOp);
            tuneTables(Eval::tablesEg);
            if (oldError - bestError < errorUncertainty) {
                break;
            } else {
                storeParams(tmpFileDest);
                std::cout << "Old Error: " << oldError << " New Error: " << bestError 
                << " Iterations: " << iterations << " Adjust Val: " << adjustVal << std::endl;
            }
        } 
        // fine tune the adjusting
        adjustVal = adjustVal >> 1;
    }
    std::cout << "Final Params Stored with: " << bestError << std::endl;
    storeParams(finalFileDest);
}

void tuneTables(std::array<std::array<int, 64UL>, 6UL>& PSQT) {
    double newError = 0.0;
    for (auto& pieceArr: PSQT) {
        for (int& i: pieceArr) {
            ++iterations;
            // try moving up by adjust val
            i += adjustVal;
            newError = meanSquareError();
            if (newError < bestError) {
                bestError = newError;
                continue;
            } 

            // try moving down by adjust val
            i -= 2 * adjustVal;
            newError = meanSquareError();
            if (newError < bestError) {
                bestError = newError;
                continue;
            }

            // reset to starting if both directions perform worse
            i += adjustVal;
        }
    }
}

double meanSquareError() {
    std::string fen, result;
    double error = 0.0, sigmoid;
    double eval;
    
    for (const PositionData& i: dataVals) {
        // get board score from fen
        Board board(i.fen);
        eval = board.evaluate();

        // calculate eval error
        sigmoid = 1 / (1 + pow(M_E,  K * eval));
        error += pow(i.result - sigmoid, 2);
    }

    return error / static_cast<double>(dataVals.size());
}

void extractData() {
    std::string fen, result;
    std::ifstream file(data);
    while (!file.eof()) {
        // extract values from data
        std::getline(file, fen, ';');
        file >> result;
        if (fen == "\n") {
            break;
        }

        // store
        PositionData position;
        position.fen = fen;
        position.result = getResult(result);
        dataVals.push_back(position);
    }
}

void storeParams(std::string file) {
    std::ofstream output(file);
    for (auto& gameStateTable: {Eval::tablesOp, Eval::tablesEg}) {
        for (auto& pieceArr: gameStateTable) {
            for (auto& i: pieceArr) {
                output << i << ",\n";
            }
        }
    }
}

double getResult(std::string result) {
    if (result == "[1-0]") {return 1.0;}
    else if (result == "[1/2-1/2]") {return 0.5;}
    else if (result == "[0-1]") {return 0;}
    throw std::runtime_error("Invalid result: " + result);
}