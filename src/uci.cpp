#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "uci.hpp"
#include "search.hpp"
#include "inCheck.hpp"
#include "board.hpp"


int main() {
    if (!uci()) {return 1;}
    SETOPTIONLOOP();
    UCILOOP();

    return 0;
}

bool uci() {
    std::string input;
    std::getline(std::cin, input);
    if (input != "uci") {
        std::cout << "This engine is only UCI";
        return false;
    }
    std::cout << "id name BLOCKY\n";
    std::cout << "id author BlockyTeam\n";
    std::cout << "uciok\n";
    return true;
}

void SETOPTIONLOOP() {
    std::string commandLine, commandToken;
    while (true) {
        std::getline(std::cin, commandLine);
        std::istringstream commandStream(commandLine);
        commandStream >> commandToken;

        if (commandToken == "setoption") {setoption(commandStream);}
        else if (commandToken == "isready") {isready(); break;}
    }
}

void setoption(std::istringstream& input){} // no options to set yet


void UCILOOP() {
    std::string commandLine, commandToken;
    Board currBoard;
    while (true) {
        std::getline(std::cin, commandLine);
        std::istringstream commandStream(commandLine);
        commandStream >> commandToken;

        if (commandToken == "ucinewgame") {}
        else if (commandToken == "position") {currBoard = position(commandStream);}
        else if (commandToken == "go") {go(commandStream, currBoard);}
        else if (commandToken == "isready") {isready();}
    }
}


Board position(std::istringstream& input) {
    std::string token;
    Board currBoard;

    input >> token;
    if (token == "startpos") {
        currBoard = Board();
    }
    else if (token == "fen") {
        throw std::invalid_argument("NOT FEN");
    }

    input >> token;
    if (token != "moves") {return currBoard;}
    while (input >> token) {
        currBoard = Board(currBoard, BoardMove(token));
    }
    return currBoard;
}

void go(std::istringstream& input, Board& board) {
    std::string token;

    auto result = negaMax(board, 4);
    board = Board(board, result.second);
    std::cout << "bestmove " << result.second.toStr() << "\n";
}

void isready() {
    std::cout << "readyok\n";
}