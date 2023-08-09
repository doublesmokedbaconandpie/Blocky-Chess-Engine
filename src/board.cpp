
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

std::vector<Board> moveGenerator(Board currBoard) {
    std::vector<Board> listOfMoves;
    std::vector<BoardSquare> pawns, knights, bishops, rooks, queens, kings;

    // get squares where each piece is

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++)
    }

    // add logic to find all piece types

    validPawnMoves(currBoard, listOfMoves, pawns);
    validKnightMoves(currBoard, listOfMoves, knights);
    validBishopMoves(currBoard, listOfMoves, bishops);
    validRookMoves(currBoard, listOfMoves, rooks);
    validQueenMoves(currBoard, listOfMoves, queens);
    validKingMoves(currBoard, listOfMoves, kings);
    return listOfMoves;
}

void validPawnMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& pawns) {

    // check 2nd rank for move two spaces instead of 1

    // check 5th rank for en passant

    // check 7th rank for promotions

    // get pawn moves elsewhere (ranks 3, 4, 6)

}

void validKnightMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& knights) {
    // iterate through currBoard to find the knights
}

void validBishopMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& bishops) {
    // iterate through currBoard to find the bishops
}

void validRookMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& rooks) {
    // iterate through currBoard to find the rooks
}

void validQueenMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& queens) {
    // iterate through currBoard to find the queens (could be more due to promotions)
    // repeat the logic used for rooks and bishops
}


void validKingMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& kings) {
    // get valid king moves

    // get castling
}
