#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "board.hpp"

namespace MoveGen {

struct MoveGenInfo {
    uint64_t allPieces, emptySquares, notAllies; 
    uint64_t pawnEnemies, pawnStartRank, pawnJumpRank;
    std::array<uint64_t, NUM_BITBOARDS> pieceSets;
    uint64_t castlingRights;
    bool isWhiteTurn;
};

std::vector<BoardMove> moveGenerator(Board board); // outputs board instead of board moves for future evaluation functions

template<typename Func>
void validPieceMoves(uint64_t pieces, Func pieceMoves, MoveGenInfo& colors, Board& board, std::vector<BoardMove>& validMoves);
void validPawnMoves(uint64_t pawns, MoveGenInfo& colors, Board& board, std::vector<BoardMove>& validMoves); // includes en passant

uint64_t knightMoves(int square, MoveGenInfo& info);
uint64_t bishopMoves(int square, MoveGenInfo& info);
uint64_t rookMoves(int square, MoveGenInfo& info);
uint64_t kingMoves(int square, MoveGenInfo& info);
uint64_t pawnMoves(int square, MoveGenInfo& info, bool isWhiteTurn);

} // namespace MoveGen