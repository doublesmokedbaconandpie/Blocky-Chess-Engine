#include <fstream>
#include <string>
#include <vector>

#include "board.hpp"
#include "move.hpp"

enum WinningColor {
    WHITE, DRAW, BLACK, NA
};

WinningColor getGameResult(std::ifstream& file);
std::string getStartFen(std::ifstream& file);
std::vector<std::string> getPositions(std::ifstream& file, std::string startFen, const WinningColor result);
BoardMove getMove(std::string input, Board& board);
void storeFenResults(std::ofstream& file, std::vector<std::string> fens, WinningColor result);

std::string toStr(const WinningColor result);