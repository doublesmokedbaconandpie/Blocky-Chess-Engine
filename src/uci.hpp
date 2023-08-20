#include <string>
#include <sstream>

#include "board.hpp"

int main();

bool uci();

void SETOPTIONLOOP();
void setoption(std::istringstream& input);

void UCILOOP();
Board position(std::istringstream& input);
void go(std::istringstream& input, Board& board);

void isready();
