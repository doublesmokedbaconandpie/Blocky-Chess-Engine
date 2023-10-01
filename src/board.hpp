
#pragma once

#include <array>
#include <vector>

#include "move.hpp"
#include "bitboard.hpp"
#include "types.hpp"


//this struct is purely for organization
struct EvalAttributes {
    //EvalAttributes(); //default position

    EvalAttributes(uint8_t pieceCount = 32, uint8_t totalMaterial = 78); 

    uint8_t piecesRemaining;
    int totalMaterial; //in consideration, not sure how much this would influence things
    int placementScore = 0;
    gameProgress gameState = Opening;
};

struct BoardState {
    BoardMove move = BoardMove();
    pieceTypes originPiece;
    pieceTypes targetPiece;
    castleRights castlingRights;
    BoardSquare pawnJumpedSquare;
    int fiftyMoveRule;
    int materialDifference;
    EvalAttributes eval;
    BoardState(BoardMove a_move, pieceTypes a_originPiece, pieceTypes a_targetPiece, 
                castleRights a_castlingRights, BoardSquare a_pawnJumpedSquare, int a_fiftyMoveRule,
                int a_materialDifference, EvalAttributes a_eval) : 
                move(a_move), originPiece(a_originPiece), targetPiece(a_targetPiece),
                castlingRights(a_castlingRights), pawnJumpedSquare(a_pawnJumpedSquare), fiftyMoveRule(a_fiftyMoveRule),
                materialDifference(a_materialDifference), eval(a_eval) {};
};

struct Board {
    // for debugging
    Board();
    Board(std::array<pieceTypes, BOARD_SIZE> a_board, bool a_isWhiteTurn = true, 
            int a_fiftyMoveRule = 0, BoardSquare a_pawnJumpedSquare = BoardSquare(), 
            bool a_isIllegalPos = false, castleRights a_castlingRights = All_Castle, int a_materialDifference = 0); 
    // for production
    Board(std::string fenStr);
    std::string toFen();
    void initZobristKey();
    
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
    int getEvalScore() const;

    std::array<uint64_t, NUM_BITBOARDS> pieceSets = {0ull};
    std::array<pieceTypes, BOARD_SIZE> board = {EmptyPiece};

    uint64_t zobristKey; // zobristKeyHistory also contains zobristKey
    bool isWhiteTurn;
    castleRights castlingRights; // bitwise castling rights tracker
    int fiftyMoveRule;
    bool isIllegalPos;
    BoardSquare pawnJumpedSquare; // en passant square
    int materialDifference; // updates on capture or promotion, so the eval doesn't have to calculate for each board, positive is white advantage
                            // Possibly could be combined with attributes
    EvalAttributes eval;

    std::vector<BoardState> moveHistory;
    std::vector<uint64_t> zobristKeyHistory;
};

castleRights castleRightsBit(BoardSquare finalKingPos, bool isWhiteTurn);
bool currKingInAttack(Board& board);

// for debugging
uint64_t makeBitboardFromArray(std::array<pieceTypes, BOARD_SIZE> board, int target);
