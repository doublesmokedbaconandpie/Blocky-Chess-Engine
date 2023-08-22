#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "uci.hpp"
#include "timeman.hpp"
#include "search.hpp"
#include "inCheck.hpp"
#include "board.hpp"

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

        std::cout << "option name UseDepth type check default false\n";
        std::cout << "option name Depth type spin default 1 min 1 max 4\n";

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
        if (token == "UseDepth") {
            input >> token;
            input >> token;
            OPTIONS.useDepth = token == "true" ? true : false;
        }
        if (token == "Depth" && OPTIONS.useDepth) {
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
            else if (commandToken == "go") {UCI::go(commandStream, currBoard);}
            else if (commandToken == "isready") {isready();}
            else if (commandToken == "quit") {return;}
        }
    }


    Board position(std::istringstream& input) {
        std::string token;
        Board currBoard;

        input >> token;
        if (token == "startpos") {
            currBoard = Board();
            input >> token;
        }
        else if (token == "fen") {
            std::string fenStr;
            while (input >> token && token != "moves") { 
                fenStr += token + ' ';
            }
            currBoard = Board(fenStr);
        }

        if (token != "moves") {return currBoard;}
        while (input >> token) {
            currBoard = Board(currBoard, BoardMove(token, currBoard.isWhiteTurn));
        }
        return currBoard;
    }

    void go(std::istringstream& input, Board& board) {
        int wtime, btime, allytime;
        std::string param, value;
        while (input >> param) {
            input >> value;
            if (param == "wtime") {wtime = stoi(value);}
            else if (param == "btime") {btime = stoi(value);}
        }   
        allytime = board.isWhiteTurn ? wtime : btime;
    
        std::pair<int, BoardMove> result;
        if (OPTIONS.useDepth) {result = negaMax(board, OPTIONS.depth);}
        else {result = negaMax(board, negamaxDepthBasedOnTime(allytime));}

        board = Board(board, result.second);
        if (result.first < 100000 && result.first > -1000000) {
            std::cout << "info depth " << OPTIONS.depth << " score cp " << (result.first * 100) << "\n";
        }
        std::cout << "bestmove " << result.second.toStr() << "\n";
    }

    void isready() {
        std::cout << "readyok\n";
    }

} // namespace UCI

