#pragma once

#include "board.hpp"

// each checkPiece function only checks a single piece for attacks on the enemy king
// targetKing should only be one of WKing and BKing
// checkPiece doesn't check if the piece and targetKing are on the same side

bool InCheck(Board& currBoard); // all pieces in general
bool checkPawn(Board& currBoard, BoardSquare pawn, int targetKing);
bool checkBishop(Board& currBoard, BoardSquare bishop, int targetKing);
bool checkRook(Board& currBoard, BoardSquare rook, int targetKing);
bool checkKnight(Board& currBoard, BoardSquare knight, int targetKing);
bool checkKing(Board& currBoard, BoardSquare king, int targetKing);
