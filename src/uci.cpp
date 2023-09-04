#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <chrono>

#include "uci.hpp"
#include "timeman.hpp"
#include "search.hpp"
#include "board.hpp"

namespace Uci {
    UciOptions OPTIONS;

    bool uci() {
        std::string input;
        std::getline(std::cin, input);
        if (input != "uci") {
            std::cout << "This engine is only Uci";
            return false;
        }
        std::cout << "id name BLOCKY\n";
        std::cout << "id author BlockyTeam\n";

        std::cout << "option name Depth type spin default 5 min 1 max 7\n";

        std::cout << "uciok\n";
        return true;
    }

    void setOptionLoop() {
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


    void uciLoop() {
        std::string commandLine, commandToken;
        Board currBoard;
        while (true) {
            std::getline(std::cin, commandLine);
            std::istringstream commandStream(commandLine);
            commandStream >> commandToken;

            if (commandToken == "ucinewgame") {}
            else if (commandToken == "position") {currBoard = position(commandStream);}
            else if (commandToken == "go") {Uci::go(commandStream, currBoard);}
            else if (commandToken == "isready") {isready();}
            else if (commandToken == "quit") {return;}
        }
    }


    Board position(std::istringstream& input) {
        std::string token;
        Board currBoard;

        input >> token;
        if (token == "startpos") {
            input >> token;
            currBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
            currBoard.makeMove(BoardMove(token, currBoard.isWhiteTurn));
        }
        return currBoard;
    }

    void go(std::istringstream& input, Board& board) {
        std::string token;
        int wtime = TIMEMAN::INF_TIME, btime = TIMEMAN::INF_TIME, allytime;
        std::string param, value;
        while (input >> param) {
            input >> value;
            if (param == "wtime") {wtime = stoi(value);}
            else if (param == "btime") {btime = stoi(value);}
        }   
        allytime = board.isWhiteTurn ? wtime : btime;
        int depthToUse = OPTIONS.depth < TIMEMAN::timeToDepth(allytime) ? OPTIONS.depth : TIMEMAN::timeToDepth(allytime);

        auto start = std::chrono::high_resolution_clock::now();
        Search::SearchInfo result = Search::search(board, depthToUse);
        auto end = std::chrono::high_resolution_clock::now();
        int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        
        info(result, duration, depthToUse);
        std::cout << "bestmove " << result.move.toStr() << "\n";
    }

    void info(Search::SearchInfo searchResult, int64_t searchDuration, int depth) {
        std::cout << "info depth " << depth << ' ';
        std::cout << "nodes " << searchResult.nodes << ' ';
        if (searchDuration != 0) {
            std::cout << "nps " << static_cast<int64_t>(searchResult.nodes) * 1000000 / searchDuration  << ' ';
        }
        
        if (searchResult.mateIn == Search::NO_MATE) {
            std::cout << "score cp " << (searchResult.value * 100) << ' ';
        }
        else {
            std::cout << "mate " << searchResult.mateIn / 2 + 1 << ' '; // convert plies to moves
        }
        std::cout << '\n';
    }

    void isready() {
        std::cout << "readyok\n";
    }

} // namespace Uci

