#pragma once

#include "board.hpp"

// each checkPiece function only checks a single piece for attacks on the enemy king
// targetKing should only be one of WKing and BKing
// checkPiece doesn't check if the piece and targetKing are on the same side

bool isWhite(pieceTypes piece);
bool sameSide(pieceTypes piece1, pieceTypes piece2);

void addMovesInDirection(Board& currBoard, std::vector<BoardSquare>& movesVec, BoardSquare originSquare, int rankIncrement, int fileIncrement);
pieceTypes checkPieceInDirection(Board& currBoard, BoardSquare originSquare, int rankIncrement, int fileIncrement);
bool checkDiagPin(pieceTypes origin, pieceTypes endPiece1, pieceTypes endPiece2);
bool checkStraightPin(pieceTypes origin, pieceTypes endPiece1, pieceTypes endPiece2);
