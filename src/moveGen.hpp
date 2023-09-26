#pragma once

#include <cstdint>
#include <vector>

#include "board.hpp"

namespace MOVEGEN {

    std::vector<BoardMove> moveGenerator(Board currBoard); // outputs board instead of board moves for future evaluation functions
    void validPawnMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t pawns); // includes en passant
    void validKnightMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t knights);
    void validBishopMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t bishops);
    void validRookMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t rooks);
    void validQueenMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t queens);
    void validKingMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t kings); // includes castling

    void addMovesInDirection(Board& currBoard, std::vector<BoardSquare>& movesVec, BoardSquare originSquare, int rankIncrement, int fileIncrement);
    pieceTypes getPieceInDirection(Board& currBoard, BoardSquare originSquare, int rankIncrement, int fileIncrement);
    bool isFriendlyPiece(Board& currBoard, BoardSquare targetSquare);

    // for debugging 
    uint64_t perft(Board currBoard, int depthLeft);
} // namespace MOVEGEN