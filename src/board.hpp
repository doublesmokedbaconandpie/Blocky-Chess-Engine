#pragma once

#include <array>
#include <vector>

#include "eval.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "types.hpp"

struct BoardState {
    BoardMove move;
    pieceTypes originPiece;
    pieceTypes targetPiece;
    castleRights castlingRights;
    Square enPassSquare;
    int fiftyMoveRule;
    BoardState(BoardMove a_move, pieceTypes a_originPiece, pieceTypes a_targetPiece, 
                castleRights a_castlingRights, Square a_enPassSquare, int a_fiftyMoveRule) : 
                move(a_move), originPiece(a_originPiece), targetPiece(a_targetPiece),
                castlingRights(a_castlingRights), enPassSquare(a_enPassSquare), fiftyMoveRule(a_fiftyMoveRule) {};
};

struct Board {
    Board(std::string fenStr);
    Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {};
    std::string toFen();
    void initZobristKey();
    
    void makeMove(BoardMove move);
    void undoMove();
    void makeNullMove();
    void unmakeNullMove();
    
    pieceTypes getPiece(Square square) const;
    void setPiece(Square square, pieceTypes currPiece);
    bool moveIsCapture(BoardMove move) const;
    
    bool isLegalMove(const BoardMove move) const;
    int evaluate() const;

    friend bool operator==(const Board& lhs, const Board& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Board& target);

    std::array<uint64_t, NUM_BITBOARDS> pieceSets = {0ull};
    std::array<pieceTypes, BOARD_SIZE> board = {EmptyPiece};

    uint64_t zobristKey; // zobristKeyHistory also contains zobristKey
    bool isWhiteTurn;
    castleRights castlingRights; // bitwise castling rights tracker
    int fiftyMoveRule;
    int age = 0;
    Square enPassSquare; // en passant square
    Eval::Info eval;

    std::vector<BoardState> moveHistory;
    std::vector<uint64_t> zobristKeyHistory;
};

castleRights castleRightsBit(Square finalKingPos, bool isWhiteTurn);
bool currKingInAttack(const std::array<uint64_t, NUM_BITBOARDS>& pieceSets, bool isWhiteTurn);
