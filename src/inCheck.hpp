#pragma once

#include "board.hpp"

// each checkPiece function only checks a single piece for attacks on the enemy king
// targetKing should only be one of WKing and BKing
// checkPiece doesn't check if the piece and targetKing are on the same side

bool isFriendlyPiece(Board& currBoard, BoardSquare targetSquare);
pieceTypes getPieceInDirection(Board& currBoard, BoardSquare originSquare, int rankIncrement, int fileIncrement);

bool checkDiagAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece);
bool checkStraightAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece);
bool checkKnightAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece);
bool checkPawnAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece);
bool checkKingAttackers(Board& currBoard, BoardSquare originSquare, pieceTypes originPiece);

bool notInRange(int var);
bool currKingInAttackAfterMove(Board currBoard);
