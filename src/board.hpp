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
        bool moveIsCapture(BoardMove move) const;
        
        pieceTypes getPiece(Square square) const;
        void setPiece(Square square, pieceTypes currPiece);
        
        bool isLegalMove(const BoardMove move) const;
        bool isDraw() const;
        int evaluate();
        void clearHistory();

        auto getZobristKey() const -> uint64_t;
        auto getFiftyMoveRule() const -> int;

        friend bool operator==(const Board& lhs, const Board& rhs);
        friend std::ostream& operator<<(std::ostream& os, const Board& target);

        PieceSets pieceSets = {0ull};
        std::array<pieceTypes, BOARD_SIZE> board = {EmptyPiece};

        bool isWhiteTurn;
        castleRights castlingRights; // bitwise castling rights tracker
        int age = 0;
        Square enPassSquare; // en passant square

        std::vector<BoardState> moveHistory;

    private:
        void initZobristKey();

        Eval::Info eval;
        int fiftyMoveRule;
        uint64_t zobristKey; // zobristKeyHistory also contains zobristKey
        std::vector<uint64_t> zobristKeyHistory;
};

inline auto Board::getZobristKey() const -> uint64_t {
    return this->zobristKey;
}

inline auto Board::getFiftyMoveRule() const -> int {
    return this->fiftyMoveRule;
}

castleRights castleRightsBit(Square finalKingPos, bool isWhiteTurn);
bool currKingInAttack(const PieceSets& pieceSets, bool isWhiteTurn);
uint64_t getAllPieces(const PieceSets& pieceSets);
