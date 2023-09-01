#pragma once

#include <vector>

#include "board.hpp"

namespace MOVEGEN {

    std::vector<BoardMove> moveGenerator(Board currBoard); // outputs board instead of board moves for future evaluation functions
    void validPawnMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& pawns); // includes en passant
    void validKnightMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& knights);
    void validBishopMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& bishops);
    void validRookMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& rooks);
    void validQueenMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& queens);
    void validKingMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& kings); // includes castling

    void addMovesInDirection(Board& currBoard, std::vector<BoardSquare>& movesVec, BoardSquare originSquare, int rankIncrement, int fileIncrement);
    pieceTypes getPieceInDirection(Board& currBoard, BoardSquare originSquare, int rankIncrement, int fileIncrement);
    bool isFriendlyPiece(Board& currBoard, BoardSquare targetSquare);

    void forwardPawnMoves(Board& currBoard, std::vector<BoardSquare>& pawnMoves, BoardSquare pawn);
    void pawnCaptures(Board& currBoard, std::vector<BoardSquare>& pawnMoves, BoardSquare pawn, int fileDirection);

} // namespace MOVEGEN