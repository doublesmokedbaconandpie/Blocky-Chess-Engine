#include <array>
#include <string>

#include "board.hpp"

struct PositionData {
    Board board;
    double result;
    double eval;
};

double computeOptK();
double meanSquareError(double K);
void extractData();
void storeParams(std::string file);
double getResult(std::string result);

double linearEval();
void updateSingleGradient();
void computeGradient();