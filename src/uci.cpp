#include <iostream>
#include <string>
#include <stdexcept>

#include "uci.hpp"
#include "search.hpp"
#include "inCheck.hpp"
#include "board.hpp"

int main() {
    std::string input;

    bool runProgram = uci();
    if (!runProgram) {return 1;}
    setOptions();
    Board startBoard = ucinewgame();
    while (runProgram) {
        runProgram = go(startBoard);
        oppMove(startBoard);
    }

    return 0;
}


bool uci() {
    std::string input;
    std::cin >> input;
    if (input != "uci") {
        std::cout << "This engine is only UCI";
        return false;
    }
    std::cout << "id name BLOCKY\n";
    std::cout << "id author BlockyTeam\n";
    std::cout << "uciok\n";
    return true;
}


void setOptions() {
    std::string input;
    std::cin >> input;
    while (input != "isready") {
        std::cin >> input;
    }
    std::cout << "readyok";
}


Board ucinewgame() {
    // does not support fenstring yet
    std::string input;
    while (input.substr(0, 8) != "position") {
        std::cin >> input;
    }
    if (input.substr(9, 8) != "startpos") {
        throw std::invalid_argument("Fenstrings not supported by Blocky");
    }

    std::string subInput = input;
    size_t index = subInput.find("moves") + 5;
    std::string moveString;
    BoardMove currMove;
    Board currBoard = Board();
    while (index != std::string::npos) {
        subInput = subInput.substr(index + 1, subInput.length() - index - 1);
        index = subInput.find(' ');
        moveString = subInput.substr(0, index);
        currMove = BoardMove(moveString, currBoard.isWhiteTurn);
        currBoard = Board(currBoard, currMove);
    }
    return currBoard;
}

bool go(Board& board) {
    auto result = negaMax(board, 3);
    board = Board(board, result.second);
    std::cout << "bestmove " << result.second.toStr() << "\n";
    return true;
}

bool oppMove(Board& board) {
    std::string input;
    std::cin >> input;
    return true;    

}