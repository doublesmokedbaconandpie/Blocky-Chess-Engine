#include <array>
#include <string>

struct PositionData {
    std::string fen;
    double result;
};

void tuneTables(std::array<std::array<int, 64UL>, 6UL>& PSQT);
double meanSquareError();
void extractData();
void storeParams(std::string file);
double getResult(std::string result);