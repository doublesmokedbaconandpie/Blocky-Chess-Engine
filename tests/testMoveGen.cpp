#include "moveGen.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

TEST(MoveGen, validPawnMoves1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {BPawn     , EmptyPiece, BPawn     , EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, WPawnJumped, EmptyPiece, WPawn    , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whitePawns = {BoardSquare(4, B), BoardSquare(4, D), BoardSquare(5, G)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(4, B), BoardSquare(3, A)),
        Board(board, BoardSquare(4, B), BoardSquare(3, B)),
        Board(board, BoardSquare(4, B), BoardSquare(3, C)),
        Board(board, BoardSquare(4, D), BoardSquare(3, C)),
        Board(board, BoardSquare(4, D), BoardSquare(3, D)),
    };
    validPawnMoves(board, validMoves, whitePawns);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGen, validPawnMoves2) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {BPawn     , EmptyPiece, BPawn     , EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, WPawnJumped, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whitePawns = {BoardSquare(4, B), BoardSquare(4, D)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(4, B), BoardSquare(3, A)),
        Board(board, BoardSquare(4, B), BoardSquare(3, C)),
        Board(board, BoardSquare(4, D), BoardSquare(3, C)),
    };
    validPawnMoves(board, validMoves, whitePawns);
    
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGen, validRookMoves1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, WRook     , WPawn     , EmptyPiece, EmptyPiece, BBishop, EmptyPiece, EmptyPiece},
        {EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, BQueen      , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteRooks = {BoardSquare(4, B)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(4, B), BoardSquare(5, B)),
        Board(board, BoardSquare(4, B), BoardSquare(4, A)),
        Board(board, BoardSquare(4, B), BoardSquare(3, B)),
        Board(board, BoardSquare(4, B), BoardSquare(2, B)),
        Board(board, BoardSquare(4, B), BoardSquare(1, B)),
    };
    validRookMoves(board, validMoves, whiteRooks);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());

    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGen, validBishopMoves1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, WRook     , WPawn     , EmptyPiece, EmptyPiece, BBishop, EmptyPiece, EmptyPiece},
        {EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, BQueen      , EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteRooks = {BoardSquare(4, B)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(4, B), BoardSquare(5, B)),
        Board(board, BoardSquare(4, B), BoardSquare(4, A)),
        Board(board, BoardSquare(4, B), BoardSquare(3, B)),
        Board(board, BoardSquare(4, B), BoardSquare(2, B)),
        Board(board, BoardSquare(4, B), BoardSquare(1, B)),
    };
    validRookMoves(board, validMoves, whiteRooks);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());

    ASSERT_EQ(validMoves, expectedValidMoves);
}

