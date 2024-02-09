#include <fstream>
#include <string>
#include <vector>

#include "board.hpp"
#include "move.hpp"

enum WinningColor {
    WHITE, DRAW, BLACK, NA
};

void readFileNames(std::vector<std::string>& pgns, std::vector<std::string>& dests);
void extractFromFile(std::string pgn, std::string destName, int& games);

WinningColor getGameResult(std::ifstream& file);
std::string getStartFen(std::ifstream& file, bool& blackStarts);
std::vector<std::string> getPositions(std::ifstream& file, std::string startFen, const WinningColor result, bool blackStarts);

Move getMove(std::string input, Board& board);
void storeFenResults(std::ofstream& file, std::vector<std::string> fens, WinningColor result);
std::string toStr(const WinningColor result);