#pragma once

#include "board.hpp"
#include "inCheck.hpp"

std::vector<Board> moveGenerator(Board currBoard); // outputs board instead of board moves for future evaluation functions
void validPawnMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& pawns); // includes en passant
void validKnightMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& knights);
void validBishopMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& bishops);
void validRookMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& rooks);
void validQueenMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& queens);
void validKingMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& kings); // includes castling
// idk what to do about 3-fold repetition; maybe a hash table

void addMovesInDirection(Board& currBoard, std::vector<BoardSquare>& movesVec, BoardSquare originSquare, int rankIncrement, int fileIncrement);
