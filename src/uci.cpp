#include <iostream>
#include <string>

#include "uci.hpp"
#include "search.hpp"
#include "board.hpp"

int main() {
    Board board = Board();
    BoardMove move = BoardMove(BoardSquare(6, 2), BoardSquare(5, 2));
    int eval;
    int moveCount = 0;

    while (true) {
        std::pair<int, BoardMove> returnVals = negaMax(board, 2);
        eval = board.isWhiteTurn ? returnVals.first : -1 * returnVals.first;
        move = returnVals.second;
        board = Board(board, move);
        moveCount++;

        if (move == BoardMove()) {
            break;
        }
        
        if (board.isWhiteTurn) {
            std::cout << moveCount << ". Black Moved:" << std::endl;
        }
        else {
            std::cout << moveCount << ". White Moved:" << std::endl;
        }
        std::cout << "Evaluation: " << eval << std::endl;
        std::cout << "Best Move: " << move << std::endl;
        std::cout << "Best Board: \n \n" << board << std::endl << std::endl;

    }        
    return 0;
}