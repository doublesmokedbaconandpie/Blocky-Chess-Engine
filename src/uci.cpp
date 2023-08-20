#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "uci.hpp"
#include "search.hpp"
#include "inCheck.hpp"
#include "board.hpp"


int main() {
    if (!UCI::uci()) {return 1;}
    UCI::SETOPTIONLOOP();
    UCI::UCILOOP();

    return 0;
}

namespace UCI {
    UCIOPTIONS OPTIONS;

    bool uci() {
        std::string input;
        std::getline(std::cin, input);
        if (input != "uci") {
            std::cout << "This engine is only UCI";
            return false;
        }
        std::cout << "id name BLOCKY\n";
        std::cout << "id author BlockyTeam\n";

        std::cout << "option name Depth type spin default 3 min 1 max 4\n";

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

    void setoption(std::istringstream& input){ 
        std::string token;
        input >> token; 
        if (token != "name") {return;}
        input >> token;
        if (token == "Depth") {
            input >> token;
            input >> token;
            OPTIONS.depth = stoi(token);
            std::cout << "Depth set to: " << OPTIONS.depth << std::endl;
        };
    }


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

        auto result = negaMax(board, OPTIONS.depth);
        board = Board(board, result.second);
        std::cout << "bestmove " << result.second.toStr() << "\n";
    }

    void isready() {
        std::cout << "readyok\n";
    }

} // namespace UCI

