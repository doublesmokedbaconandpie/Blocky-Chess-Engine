#pragma once

#include "board.hpp"

// each checkPiece function only checks a single piece for attacks on the enemy king
// targetKing should only be one of WKing and BKing
// checkPiece doesn't check if the piece and targetKing are on the same side

bool isFriendlyPiece(Board& currBoard, BoardSquare targetSquare);
void addMovesInDirection(Board& currBoard, std::vector<BoardSquare>& movesVec, BoardSquare originSquare, int rankIncrement, int fileIncrement);
pieceTypes getPieceInDirection(Board& currBoard, BoardSquare originSquare, int rankIncrement, int fileIncrement);
bool checkDiagAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece);
bool checkStraightAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece);
bool isInCheck(Board currBoard);