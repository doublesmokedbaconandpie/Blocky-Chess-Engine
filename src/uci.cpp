#include <cctype>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "uci.hpp"
#include "timeman.hpp"
#include "ttable.hpp"
#include "search.hpp"
#include "moveGen.hpp"
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

        std::cout << "option name maxDepth type spin default 100 min 1 max 200\n";
        std::cout << "option name Hash type spin default 128 min 128 max 1024\n";

        std::cout << "uciok\n";
        return true;
    }

    void setOptionLoop() {
        std::string commandLine, commandToken;
        while (true) {
            std::getline(std::cin, commandLine);
            std::istringstream commandStream(commandLine);
            commandStream >> commandToken;

            if (commandToken == "setoption") {setOption(commandStream);}
            else if (commandToken == "isready") {isready(); break;}
        }
    }

    void setOption(std::istringstream& input){ 
        // Example: setoption name maxDepth value 2

        // gather inputs
        std::string token, id, value;
        input >> token; // name qualifier
        input >> id;
        input >> token; // value qualifier
        input >> value;

        // uci requires id to not be case sensitive
        std::transform(id.begin(), id.end(), id.begin(), ::tolower);

        if (id == "maxdepth") {
            OPTIONS.depth = std::stoi(value);
        }
        else if (id == "hash") {
            TTable::table.resize(std::stoi(value));
        }
    }

    void uciLoop() {
        std::string commandLine, commandToken;
        Board currBoard;
        while (true) {
            std::getline(std::cin, commandLine);
            std::istringstream commandStream(commandLine);
            commandStream >> commandToken;

            if (commandToken == "ucinewgame") {TTable::table.clear();}
            else if (commandToken == "position") {currBoard = position(commandStream);}
            else if (commandToken == "go") {Uci::go(commandStream, currBoard);}
            else if (commandToken == "isready") {isready();}
            else if (commandToken == "perft") {perft(commandStream, currBoard);}
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
            // if a capture or castling rights change, clear move history since
            // 3fold repetition or 50 move rule will be reset
            // this makes things faster
            if (currBoard.moveHistory.back().targetPiece != EmptyPiece ||
                currBoard.castlingRights != currBoard.moveHistory.back().castlingRights) {
                currBoard.moveHistory.clear();
                currBoard.zobristKeyHistory.clear();
                currBoard.zobristKeyHistory.push_back(currBoard.zobristKey);
            }
        }
        return currBoard;
    }

    void go(std::istringstream& input, Board& board) {
        std::string token;
        int wtime = Timeman::INF_TIME, btime = Timeman::INF_TIME, allytime;
        std::string param, value;
        while (input >> param) {
            input >> value;
            if (param == "wtime") {wtime = std::stoi(value);}
            else if (param == "btime") {btime = std::stoi(value);}
        }   
        allytime = board.isWhiteTurn ? wtime : btime;

        Search::Searcher currSearch(board, allytime, OPTIONS.depth);
        Search::Info result = currSearch.startThinking();
        std::cout << "bestmove " << result.move.toStr() << "\n";
    }

    void isready() {
        std::cout << "readyok\n";
    }

    void perft(std::istringstream& input, Board& board) {
        // validate arguments
        std::string token;
        int depth;
        if (!(input >> token)) {
            std::cout << "ARGUMENT ERROR: Perft requires a depth to search to" << std::endl;
            return;
        }
        try {
            depth = std::stoi(token);
        } 
        catch(std::exception& e) {
            std::cout << "ARGUMENT ERROR: Perft requires an integer to search to" << std::endl;
            return;
        }
        
        // perform perft
        auto start = std::chrono::high_resolution_clock::now();
        uint64_t nodes = MoveGen::perft(board, depth);
        auto end = std::chrono::high_resolution_clock::now();
        int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "perft result nodes " << nodes;
        std::cout << " nps " << nodes * 1000000 / duration;
        std::cout << " time " << duration / 1000 << "\n";

    }
} // namespace Uci

