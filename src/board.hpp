#pragma once

#include <array>
#include <vector>

#include "eval.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "types.hpp"

struct BoardState {
    BoardMove move = BoardMove();
    pieceTypes originPiece;
    pieceTypes targetPiece;
    castleRights castlingRights;
    BoardSquare pawnJumpedSquare;
    int fiftyMoveRule;
    int materialDifference;
    Eval::Info eval;
    BoardState(BoardMove a_move, pieceTypes a_originPiece, pieceTypes a_targetPiece, 
                castleRights a_castlingRights, BoardSquare a_pawnJumpedSquare, int a_fiftyMoveRule,
                int a_materialDifference, Eval::Info a_eval) : 
                move(a_move), originPiece(a_originPiece), targetPiece(a_targetPiece),
                castlingRights(a_castlingRights), pawnJumpedSquare(a_pawnJumpedSquare), fiftyMoveRule(a_fiftyMoveRule),
                materialDifference(a_materialDifference), eval(a_eval) {};
};

struct Board {
    Board(std::string fenStr);
    Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {};
    std::string toFen();
    void initZobristKey();
    
    void makeMove(BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece = nullPiece);
    void makeMove(BoardMove move);
    void undoMove();
    
    pieceTypes getPiece(int rank, int file) const;
    pieceTypes getPiece(BoardSquare square) const;
    void setPiece(int rank, int file, pieceTypes currPiece);
    void setPiece(BoardSquare square, pieceTypes currPiece);
    bool moveIsCapture(BoardMove move);
    
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
    BoardSquare pawnJumpedSquare; // en passant square
    int materialDifference; // updates on capture or promotion, so the eval doesn't have to calculate for each board, positive is white advantage
                            // Possibly could be combined with attributes
    Eval::Info eval;

    std::vector<BoardState> moveHistory;
    std::vector<uint64_t> zobristKeyHistory;
};

castleRights castleRightsBit(BoardSquare finalKingPos, bool isWhiteTurn);
bool currKingInAttack(const std::array<uint64_t, NUM_BITBOARDS>& pieceSets, bool isWhiteTurn);
