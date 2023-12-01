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
constexpr double KPRECISION = 10;

// global variables
std::vector<PositionData> dataVals;

int main() {
    // init
    Attacks::init();
    extractData();
    const double K = computeOptK();

    return 0;
}

double computeOptK(){
    double start = 0, end = 10, step = 1;
    double curr = start, currErr;
    double bestErr = meanSquareError(start);

    for (int i = 0; i < KPRECISION; ++i) {

        // find the minimum between the current range: [start, end]
        curr = start - step;
        while (curr < end) {
            curr += step;
            currErr = meanSquareError(curr);
            if (currErr <= bestErr) {
                bestErr = currErr;
                start = curr;
            }
        }

        // adjust the search space
        end = start + step;
        start -= step;
        step /= 10;

        // report status
        std::cout << "Computing K: " << "iter = " << i << ", K = " << start << '\n';
    }

    return start;
}

double meanSquareError(double K) {
    double error = 0.0, sigmoid;
    for (const auto& i: dataVals) {
        sigmoid = 1 / (1 + pow(M_E,  K * i.eval));
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
        position.board = Board(fen);
        position.result = getResult(result);
        position.eval = position.board.evaluate();
        dataVals.push_back(position);
    }
}

void storeParams(std::string file) {
    std::ofstream output(file);
    for (const auto& gameStateTable: {Eval::tablesOp, Eval::tablesEg}) {
        for (const auto& pieceArr: gameStateTable) {
            for (const auto& i: pieceArr) {
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
