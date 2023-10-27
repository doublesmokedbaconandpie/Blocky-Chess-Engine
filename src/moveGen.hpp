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

// indexes based on castle rights defined in "types.hpp"
constexpr std::array<uint64_t, 4> rookPaths = {
    0x6000000000000000ull, 0x0E00000000000000ull, 0x0000000000000060ull, 0x000000000000000Eull};
constexpr std::array<uint64_t, 4> kingPaths = {
    0x2000000000000000ull, 0x0800000000000000ull, 0x0000000000000020ull, 0x0000000000000008ull};
constexpr std::array<uint64_t, 4> castleDestination = {
    0x4000000000000000ull, 0x0400000000000000ull, 0x0000000000000040ull, 0x0000000000000004ull};

// for debugging
uint64_t perft(Board board, int depthLeft);
uint64_t perftHelper(Board board, int depthLeft);


} // namespace MoveGen