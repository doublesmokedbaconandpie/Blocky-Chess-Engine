#include "moveGen.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

// there must be a king in each test to see for illegal moves

TEST(MoveGenTest, validPawnMovesCaptures) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {BPawn     , EmptyPiece, BPawn     , EmptyPiece, WQueen    , EmptyPiece, EmptyPiece, EmptyPiece},
        {WKing     , WPawnJumped, EmptyPiece, WPawn    , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whitePawns = {BoardSquare(4, B), BoardSquare(4, D), BoardSquare(6, G)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(4, B), BoardSquare(3, A)),
        BoardMove(BoardSquare(4, B), BoardSquare(3, B)),
        BoardMove(BoardSquare(4, B), BoardSquare(3, C)),

        BoardMove(BoardSquare(4, D), BoardSquare(3, C)),
        BoardMove(BoardSquare(4, D), BoardSquare(3, D)),

        BoardMove(BoardSquare(6, G), BoardSquare(5, G)),
        BoardMove(BoardSquare(6, G), BoardSquare(4, G)),
    };
    validPawnMoves(board, validMoves, whitePawns);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGenTest, validPawnMovesPromotion) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whitePawns = {BoardSquare(1, D)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(1, D), BoardSquare(0, D), WKnight),
        BoardMove(BoardSquare(1, D), BoardSquare(0, D), WBishop),
        BoardMove(BoardSquare(1, D), BoardSquare(0, D), WRook),
        BoardMove(BoardSquare(1, D), BoardSquare(0, D), WQueen),
    };
    validPawnMoves(board, validMoves, whitePawns);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGenTest, validKnightMoves1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece},
        {WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteKnights = {BoardSquare(7, A), BoardSquare(4, D)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(7, A), BoardSquare(5, B)),
        BoardMove(BoardSquare(7, A), BoardSquare(6, C)),
        
        BoardMove(BoardSquare(4, D), BoardSquare(6, C)),
        // BoardMove(BoardSquare(4, D), BoardSquare(6, E)), King located here
        BoardMove(BoardSquare(4, D), BoardSquare(2, C)),
        BoardMove(BoardSquare(4, D), BoardSquare(2, E)),
        BoardMove(BoardSquare(4, D), BoardSquare(5, F)),
        BoardMove(BoardSquare(4, D), BoardSquare(5, B)),
        BoardMove(BoardSquare(4, D), BoardSquare(3, F)),
        BoardMove(BoardSquare(4, D), BoardSquare(3, B)),
    };

    validKnightMoves(board, validMoves, whiteKnights);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGenTest, validRookMoves1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {WBishop   , WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WRookUnmoved, WRook   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BRook     },
    };
    Board board = Board(boardVector, true);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteRooks = {BoardSquare(7, A), BoardSquare(7, B)};
    std::vector<BoardMove> expectedValidMoves;
    for (int rank = 6; rank >= 1; rank--) {
        expectedValidMoves.push_back(BoardMove(BoardSquare(7, A), BoardSquare(rank, A)));
        expectedValidMoves.push_back(BoardMove(BoardSquare(7, B), BoardSquare(rank, B)));
    }
    for (int file = H; file >= C; file--) {
        expectedValidMoves.push_back(BoardMove(BoardSquare(7, B), BoardSquare(7, file)));
    }
    validRookMoves(board, validMoves, whiteRooks);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGenTest, validBishopMoves1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BBishop   , EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WBishop   , WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteBishops = {BoardSquare(7, A), BoardSquare(4, D)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(7, A), BoardSquare(6, B)),
        BoardMove(BoardSquare(7, A), BoardSquare(5, C)),

        BoardMove(BoardSquare(4, D), BoardSquare(5, C)),
        BoardMove(BoardSquare(4, D), BoardSquare(6, B)),

        BoardMove(BoardSquare(4, D), BoardSquare(3, C)),
        BoardMove(BoardSquare(4, D), BoardSquare(2, B)),
        BoardMove(BoardSquare(4, D), BoardSquare(1, A)),

        BoardMove(BoardSquare(4, D), BoardSquare(5, E)),
        BoardMove(BoardSquare(4, D), BoardSquare(6, F)),
        BoardMove(BoardSquare(4, D), BoardSquare(7, G)),

        BoardMove(BoardSquare(4, D), BoardSquare(3, E)),
        BoardMove(BoardSquare(4, D), BoardSquare(2, F)),
    };
    validBishopMoves(board, validMoves, whiteBishops);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGenTest, validQueenMoves1) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WQueen    , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WQueen    },
    };
    Board board = Board(boardVector);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteQueens = {BoardSquare(7, A), BoardSquare(7, H)};
    std::vector<BoardMove> expectedValidMoves;
    for (int i = 1; i <= 7; i++) {
        expectedValidMoves.push_back(BoardMove(BoardSquare(7, A), BoardSquare(7 - i, A)));
        expectedValidMoves.push_back(BoardMove(BoardSquare(7, A), BoardSquare(7 - i, A + i)));

        expectedValidMoves.push_back(BoardMove(BoardSquare(7, H), BoardSquare(7 - i, H)));
        expectedValidMoves.push_back(BoardMove(BoardSquare(7, H), BoardSquare(7 - i, H - i)));
    }
    for (int i = 1; i <= 6; i++) {
        expectedValidMoves.push_back(BoardMove(BoardSquare(7, A), BoardSquare(7, A + i)));
        expectedValidMoves.push_back(BoardMove(BoardSquare(7, H), BoardSquare(7, H - i)));
    }
    validQueenMoves(board, validMoves, whiteQueens);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGenTest, validKingMovesNoCastle) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WKingUnmoved, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
    };
    Board board = Board(boardVector);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, A), BoardSquare(4, D)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(7, A), BoardSquare(6, A)),
        BoardMove(BoardSquare(7, A), BoardSquare(6, B)),
        BoardMove(BoardSquare(7, A), BoardSquare(7, B)),

        BoardMove(BoardSquare(4, D), BoardSquare(3, E)),
        BoardMove(BoardSquare(4, D), BoardSquare(3, D)),
        BoardMove(BoardSquare(4, D), BoardSquare(3, C)),
        BoardMove(BoardSquare(4, D), BoardSquare(5, E)),
        BoardMove(BoardSquare(4, D), BoardSquare(5, D)),
        BoardMove(BoardSquare(4, D), BoardSquare(5, C)),
        BoardMove(BoardSquare(4, D), BoardSquare(4, E)),
        BoardMove(BoardSquare(4, D), BoardSquare(4, C)),
    };
    validKingMoves(board, validMoves, whiteKings);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGenTest, validKingMovesKingCastle) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WRookUnmoved, EmptyPiece, WBishop , EmptyPiece, WKingUnmoved, EmptyPiece, EmptyPiece, WRookUnmoved},
    };
    Board board = Board(boardVector);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, E)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(7, E), BoardSquare(6, D)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, E)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, F)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, D)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, F)),

        BoardMove(BoardSquare(7, E), BoardSquare(7, H)),
    };
    validKingMoves(board, validMoves, whiteKings);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);    
}

TEST(MoveGenTest, validKingMovesQueenCastle) {
    std::vector<std::vector<pieceTypes>> boardVector = {
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WRookUnmoved, EmptyPiece, EmptyPiece, EmptyPiece, WKingUnmoved, EmptyPiece, WBishop, WRookUnmoved},
    };
    Board board = Board(boardVector);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, E)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(7, E), BoardSquare(6, D)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, E)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, F)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, D)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, F)),

        BoardMove(BoardSquare(7, E), BoardSquare(7, A)),
    };
    validKingMoves(board, validMoves, whiteKings);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);    
}

TEST(MoveGenTest, moveGeneratorDefault) {
    Board board = Board();
    std::vector<BoardMove> expectedValidMoves;
    for (int file = A; file <= H; file++) {
        expectedValidMoves.push_back(BoardMove(BoardSquare(6, file), BoardSquare(5, file)));
        expectedValidMoves.push_back(BoardMove(BoardSquare(6, file), BoardSquare(4, file)));
    }
    expectedValidMoves.push_back(BoardMove(BoardSquare(7, B), BoardSquare(5, A)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(7, B), BoardSquare(5, C)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(7, G), BoardSquare(5, F)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(7, G), BoardSquare(5, H)));
    std::vector<BoardMove> validMoves = moveGenerator(board);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(board.isWhiteTurn, true);
    ASSERT_EQ(validMoves, expectedValidMoves);
}

TEST(MoveGenTest, moveGeneratorBlackMove) {
    Board previousBoard = Board();
    Board currBoard = Board(previousBoard, BoardSquare(6, E), BoardSquare(4, E));
    std::vector<BoardMove> expectedValidMoves;
    for (int file = A; file <= H; file++) {
        expectedValidMoves.push_back(BoardMove(BoardSquare(1, file), BoardSquare(2, file)));
        expectedValidMoves.push_back(BoardMove(BoardSquare(1, file), BoardSquare(3, file)));
    }
    expectedValidMoves.push_back(BoardMove(BoardSquare(0, B), BoardSquare(2, A)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(0, B), BoardSquare(2, C)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(0, G), BoardSquare(2, F)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(0, G), BoardSquare(2, H)));
    std::vector<BoardMove> validMoves = moveGenerator(currBoard);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(currBoard.isWhiteTurn, false);
    ASSERT_EQ(validMoves, expectedValidMoves);
}