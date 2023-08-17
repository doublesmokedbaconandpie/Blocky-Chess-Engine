#include <iostream>
#include <string>
#include <chrono>

#include "uci.hpp"
#include "search.hpp"
#include "board.hpp"

int main() {
    Board board = Board();
    BoardMove move;
    // board.isWhiteTurn = false;
    int eval;
    int moveCount = 0;
    int depth;

    while (true) {

        if (board.isWhiteTurn) {
            depth = 3;
        }
        else {
            depth = 3;
        }

        auto start = std::chrono::system_clock::now();
        std::pair<int, BoardMove> returnVals = negaMax(board, depth);

        eval = board.isWhiteTurn ? returnVals.first : -1 * returnVals.first;
        move = returnVals.second;
        board = Board(board, move);
        moveCount++;

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        if (move == BoardMove()) {
            break;
        }
        
        if (board.isWhiteTurn) {
            std::cout << moveCount << ". Black Moved:\n";
        }
        else {
            std::cout << moveCount << ". White Moved:\n";
        }
        std::cout << "Time Stamp:" << std::ctime(&end_time) << "\n";
        std::cout << "Time elapsed:" << elapsed_seconds.count() << "\n";
        std::cout << "Evaluation: " << eval << "\n";
        std::cout << "Best Move: " << move << "\n";
        std::cout << "Best Board: \n \n" << board << std::endl;

    }        
    return 0;
}