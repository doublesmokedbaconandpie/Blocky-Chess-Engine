#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <chrono>

#include "uci.hpp"
#include "timeman.hpp"
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

    std::cout << "option name Depth type spin default 6 min 1 max 7\n";

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
        else if (commandToken == "perft") {perft(currBoard);}
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
    int wtime = TimeMan::INF_TIME, btime = TimeMan::INF_TIME, allytime;
    std::string param, value;
    while (input >> param) {
        input >> value;
        if (param == "wtime") {wtime = stoi(value);}
        else if (param == "btime") {btime = stoi(value);}
    }   
    allytime = board.isWhiteTurn ? wtime : btime;
    int depthToUse = OPTIONS.depth < TimeMan::timeToDepth(allytime) ? OPTIONS.depth : TimeMan::timeToDepth(allytime);

    auto start = std::chrono::high_resolution_clock::now();
    Search::Searcher currSearch(board);
    Search::Info result = currSearch.search(depthToUse);
    auto end = std::chrono::high_resolution_clock::now();
    int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    info(result, duration);
    std::cout << "bestmove " << result.move.toStr() << "\n";
}

void info(Search::Info searchResult, int64_t searchDuration) {
    std::cout << "info depth " << searchResult.depth << ' ';
    std::cout << "nodes " << searchResult.nodes << ' ';
    if (searchDuration != 0) {
        // time is output in milliseconds per the UCI protocol
        std::cout << "time " << searchDuration / 1000 << ' ';
        std::cout << "nps " << static_cast<int64_t>(searchResult.nodes) * 1000000 / searchDuration  << ' ';
    }
    
    if (searchResult.mateIn == Search::NO_MATE) {
        std::cout << "score cp " << searchResult.eval << ' ';
    }
    else { 
        std::cout << "mate " << (searchResult.mateIn + 1) / 2 << ' '; // convert plies to moves
    }
    std::cout << '\n';
}

void isready() {
    std::cout << "readyok\n";
}

void perft(Board& board) {
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t nodes = MoveGen::perft(board, OPTIONS.depth);
    auto end = std::chrono::high_resolution_clock::now();
    int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "perft result nodes " << nodes;
    std::cout << " nps " << nodes * 1000000 / duration;
    std::cout << " time " << duration / 1000 << "\n";

}

} // namespace Uci

