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
#include <vector>

#include "eval.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "utils/types.hpp"

struct BoardState {
    BoardState(BoardMove a_move, pieceTypes a_originPiece, pieceTypes a_targetPiece, castleRights a_castlingRights, Square a_enPassSquare, int a_fiftyMoveRule) : 
                move(a_move), 
                originPiece(a_originPiece), 
                targetPiece(a_targetPiece),
                castlingRights(a_castlingRights), 
                enPassSquare(a_enPassSquare), 
                fiftyMoveRule(a_fiftyMoveRule) {};
    BoardMove move;
    pieceTypes originPiece;
    pieceTypes targetPiece;
    castleRights castlingRights;
    Square enPassSquare;
    int fiftyMoveRule;
};

class Board {
    public:
        Board(std::string fenStr);
        Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {};
        std::string toFen() const;

        void makeMove(BoardMove move);
        void undoMove();
        void makeNullMove();
        void unmakeNullMove();

        pieceTypes getPiece(Square square) const;
        void setPiece(Square square, pieceTypes currPiece);

        bool isLegalMove(const BoardMove move) const;
        bool moveIsCapture(BoardMove move) const;
        bool isDraw() const;
        int evaluate();
        auto lastMoveCaptureOrCastle() const -> bool;
        void clearHistory();

        // getters
        auto board() const -> std::array<pieceTypes, BOARD_SIZE>;
        auto isWhiteTurn() const -> bool;
        auto castlingRights() const -> castleRights;
        auto enPassSquare() const -> Square;
        auto fiftyMoveRule() const -> int;
        auto age() const -> int;
        auto zobristKey() const -> uint64_t;

        bool operator==(const Board& rhs) const;
        std::ostream& operator<<(std::ostream& os) const;

        PieceSets pieceSets{};
    private:
        void initZobristKey();

        std::array<pieceTypes, BOARD_SIZE> m_board;
        Eval::Info eval;

        bool m_isWhiteTurn;
        castleRights m_castlingRights;
        Square m_enPassSquare;
        int m_fiftyMoveRule;
        int m_age = 0;
        uint64_t m_zobristKey; // zobristKeyHistory also contains zobristKey
        std::vector<uint64_t> m_zobristKeyHistory;
        std::vector<BoardState> m_moveHistory;
};

inline auto Board::board() const -> std::array<pieceTypes, BOARD_SIZE> {
    return this->m_board;
}

inline auto Board::isWhiteTurn() const -> bool {
    return this->m_isWhiteTurn;
}

inline auto Board::castlingRights() const -> castleRights {
    return this->m_castlingRights;
}

inline auto Board::enPassSquare() const -> Square {
    return this->m_enPassSquare;
}

inline auto Board::fiftyMoveRule() const -> int {
    return this->m_fiftyMoveRule;
}

inline auto Board::age() const -> int {
    return this->m_age;
}

inline auto Board::zobristKey() const -> uint64_t {
    return this->m_zobristKey;
}

castleRights castleRightsBit(Square finalKingPos, bool isWhiteTurn);
bool currKingInAttack(const PieceSets& pieceSets, bool isWhiteTurn);
uint64_t getAllPieces(const PieceSets& pieceSets);
