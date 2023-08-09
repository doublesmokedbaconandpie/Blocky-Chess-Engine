
#include "board.hpp"

Board::Board() {
    this->board = {
        {BRookUnmoved, BKnight, BBishop, BQueen, BKingUnmoved, BBishop, BKnight, BRookUnmoved},
        {BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn},
        {WRookUnmoved, WKnight, WBishop, WQueen, WKingUnmoved, WBishop, WKnight, WRookUnmoved}
    };
    this->isWhiteTurn = true;
    this->movesWithoutCapture = 0;
    this->pawnJumped = false;
    this->pawnJumpedSquare = BoardSquare();
    this->inCheck = false;
}

Board::Board(std::vector<std::vector<int>> board, bool isWhiteTurn = true, 
                int movesWithoutCapture = 0, bool pawnJumped = false, 
                BoardSquare pawnJumpedSquare = BoardSquare(), bool inCheck = false) {
    this->board = board;
    this->isWhiteTurn = isWhiteTurn;
    this-> movesWithoutCapture = movesWithoutCapture;
    this->pawnJumped = pawnJumped;
    this->pawnJumpedSquare = pawnJumpedSquare;
    this->inCheck = inCheck;
}

Board::Board(Board originalBoard, BoardSquare pos1, BoardSquare pos2) {
    // does not work for castling

    this->board = originalBoard.board;
    int tmp = this->board.at(pos1.rank).at(pos1.file);
    this->board.at(pos1.rank).at(pos1.file) = 0;
    this->board.at(pos2.rank).at(pos2.file) = tmp;

    this->isWhiteTurn = !originalBoard.isWhiteTurn;

    // need to add logic to convert pawnJumped, rookUnmoved, and kingUnmoved to their respective pieces
    // also check for captures
}

