#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "board.hpp"

class MoveList {
    public:
        MoveList() = default;
        MoveList(const Board& board);
        void generateAllMoves(const Board& board);
        void generateCaptures(const Board& board);
        void generateQuiets(const Board& board);

        std::vector<BoardMove> moves{};
    private:
        template<typename Func>
        void generatePieceMoves(uint64_t pieces, uint64_t validDests, Func pieceMoves, const Board& board);
        template<typename Func>
        void generatePawnPromotions(uint64_t pieces, uint64_t validDests, Func pieceMoves, const Board& board, const bool QUEENS);
        void generateKingCastles(const Board& board);

        uint64_t knightMoves(int square, uint64_t validDests) const;
        uint64_t bishopMoves(int square, uint64_t validDests) const;
        uint64_t rookMoves(int square, uint64_t validDests) const;
        uint64_t kingMoves(int square, uint64_t validDests) const;
        uint64_t pawnCaptures(int square, uint64_t validDests) const;
        uint64_t pawnPushes(int square, uint64_t validDests) const;
        uint64_t kingCastles(std::array<uint64_t, NUM_BITBOARDS> pieceSets);

        // used by both captures and quiets
        uint64_t pawns{}, promotingPawns{}, bishops{}, knights{}, rooks{}, queens{}, kings{};
        uint64_t allPieces{}, emptySquares{};
        bool isWhiteTurn{};
        // used by captures
        Square enPassSquare{};
        // used by quiets
        uint64_t pawnStartRank{}, pawnJumpRank{};
        uint64_t castlingRights{};
};
