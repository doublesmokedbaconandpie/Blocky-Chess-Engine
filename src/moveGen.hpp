/*
* Blocky, a UCI chess engine
* Copyright (C) 2023-2024, Kevin Nguyen
*
* Blocky is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* Blocky is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, see <https://www.gnu.org/licenses>.
*/

#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "board.hpp"
#include "utils/fixedVector.hpp"

class MoveList {
    public:
        MoveList() = default;
        MoveList(const Board& board);
        void generateAllMoves(const Board& board);
        void generateCaptures(const Board& board);
        void generateQuiets(const Board& board);

        FixedVector<BoardMove, MAX_MOVES> moves{};
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
        uint64_t kingCastles(PieceSets pieceSets);

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
