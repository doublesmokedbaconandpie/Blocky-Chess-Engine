
#pragma once

#include <array>
#include <vector>

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
    BoardState(BoardMove a_move, pieceTypes a_originPiece, pieceTypes a_targetPiece, 
                castleRights a_castlingRights, BoardSquare a_pawnJumpedSquare, int a_fiftyMoveRule,
                int a_materialDifference) : 
                move(a_move), originPiece(a_originPiece), targetPiece(a_targetPiece),
                castlingRights(a_castlingRights), pawnJumpedSquare(a_pawnJumpedSquare), fiftyMoveRule(a_fiftyMoveRule),
                materialDifference(a_materialDifference) {};
};

struct Board {
    Board(); // default game
    // for testing
    Board(std::array<pieceTypes, BOARD_SIZE> a_board, bool a_isWhiteTurn = true, 
            int a_fiftyMoveRule = 0, BoardSquare a_pawnJumpedSquare = BoardSquare(), 
            bool a_isIllegalPos = false, castleRights a_castlingRights = All_Castle, int a_materialDifference = 0); 
    Board(std::string fenStr);
    std::string toFen();
    
    // defined in inCheck.cpp
    void makeMove(BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece = nullPiece);
    void makeMove(BoardMove move);
    void undoMove();
    
    friend bool operator==(const Board& lhs, const Board& rhs);
    friend bool operator<(const Board& lhs, const Board& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Board& target);

    pieceTypes getPiece(int rank, int file) const;
    pieceTypes getPiece(BoardSquare square) const;
    void setPiece(int rank, int file, pieceTypes currPiece);
    void setPiece(BoardSquare square, pieceTypes currPiece);

    std::array<uint64_t, NUM_BITBOARDS> pieceSets = {0ull};
    std::array<pieceTypes, BOARD_SIZE> board = {EmptyPiece};
    bool isWhiteTurn;
    castleRights castlingRights; // bitwise castling rights tracker
    int fiftyMoveRule;
    bool isIllegalPos;
    BoardSquare pawnJumpedSquare;
    int materialDifference; //updates on capture or promotion, so the eval doesn't have to calculate for each board, positive is white advantage

    std::vector<BoardState> moveHistory;
};

castleRights castleRightsBit(BoardSquare finalKingPos);
bool notInRange(int var);
bool currKingInAttack(Board& board);

// for debugging
uint64_t makeBitboardFromArray(std::array<pieceTypes, BOARD_SIZE> board, int target);
