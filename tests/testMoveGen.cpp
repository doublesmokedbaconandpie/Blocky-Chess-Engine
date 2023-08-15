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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whitePawns = {BoardSquare(4, B), BoardSquare(4, D), BoardSquare(6, G)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(4, B), BoardSquare(3, A)),
        Board(board, BoardSquare(4, B), BoardSquare(3, B)),
        Board(board, BoardSquare(4, B), BoardSquare(3, C)),

        Board(board, BoardSquare(4, D), BoardSquare(3, C)),
        Board(board, BoardSquare(4, D), BoardSquare(3, D)),

        Board(board, BoardSquare(6, G), BoardSquare(5, G)),
        Board(board, BoardSquare(6, G), BoardSquare(4, G)),
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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whitePawns = {BoardSquare(1, D)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(1, D), BoardSquare(0, D), WKnight),
        Board(board, BoardSquare(1, D), BoardSquare(0, D), WBishop),
        Board(board, BoardSquare(1, D), BoardSquare(0, D), WRook),
        Board(board, BoardSquare(1, D), BoardSquare(0, D), WQueen),
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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteKnights = {BoardSquare(7, A), BoardSquare(4, D)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(7, A), BoardSquare(5, B)),
        Board(board, BoardSquare(7, A), BoardSquare(6, C)),
        
        Board(board, BoardSquare(4, D), BoardSquare(6, C)),
        // Board(board, BoardSquare(4, D), BoardSquare(6, E)), King located here
        Board(board, BoardSquare(4, D), BoardSquare(2, C)),
        Board(board, BoardSquare(4, D), BoardSquare(2, E)),
        Board(board, BoardSquare(4, D), BoardSquare(5, F)),
        Board(board, BoardSquare(4, D), BoardSquare(5, B)),
        Board(board, BoardSquare(4, D), BoardSquare(3, F)),
        Board(board, BoardSquare(4, D), BoardSquare(3, B)),
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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteRooks = {BoardSquare(7, A), BoardSquare(7, B)};
    std::vector<Board> expectedValidMoves;
    for (int rank = 6; rank >= 1; rank--) {
        expectedValidMoves.push_back(Board(board, BoardSquare(7, A), BoardSquare(rank, A)));
        expectedValidMoves.push_back(Board(board, BoardSquare(7, B), BoardSquare(rank, B)));
    }
    for (int file = H; file >= C; file--) {
        expectedValidMoves.push_back(Board(board, BoardSquare(7, B), BoardSquare(7, file)));
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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteBishops = {BoardSquare(7, A), BoardSquare(4, D)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(7, A), BoardSquare(6, B)),
        Board(board, BoardSquare(7, A), BoardSquare(5, C)),

        Board(board, BoardSquare(4, D), BoardSquare(5, C)),
        Board(board, BoardSquare(4, D), BoardSquare(6, B)),

        Board(board, BoardSquare(4, D), BoardSquare(3, C)),
        Board(board, BoardSquare(4, D), BoardSquare(2, B)),
        Board(board, BoardSquare(4, D), BoardSquare(1, A)),

        Board(board, BoardSquare(4, D), BoardSquare(5, E)),
        Board(board, BoardSquare(4, D), BoardSquare(6, F)),
        Board(board, BoardSquare(4, D), BoardSquare(7, G)),

        Board(board, BoardSquare(4, D), BoardSquare(3, E)),
        Board(board, BoardSquare(4, D), BoardSquare(2, F)),
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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteQueens = {BoardSquare(7, A), BoardSquare(7, H)};
    std::vector<Board> expectedValidMoves;
    for (int i = 1; i <= 7; i++) {
        expectedValidMoves.push_back(Board(board, BoardSquare(7, A), BoardSquare(7 - i, A)));
        expectedValidMoves.push_back(Board(board, BoardSquare(7, A), BoardSquare(7 - i, A + i)));

        expectedValidMoves.push_back(Board(board, BoardSquare(7, H), BoardSquare(7 - i, H)));
        expectedValidMoves.push_back(Board(board, BoardSquare(7, H), BoardSquare(7 - i, H - i)));
    }
    for (int i = 1; i <= 6; i++) {
        expectedValidMoves.push_back(Board(board, BoardSquare(7, A), BoardSquare(7, A + i)));
        expectedValidMoves.push_back(Board(board, BoardSquare(7, H), BoardSquare(7, H - i)));
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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, A), BoardSquare(4, D)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(7, A), BoardSquare(6, A)),
        Board(board, BoardSquare(7, A), BoardSquare(6, B)),
        Board(board, BoardSquare(7, A), BoardSquare(7, B)),

        Board(board, BoardSquare(4, D), BoardSquare(3, E)),
        Board(board, BoardSquare(4, D), BoardSquare(3, D)),
        Board(board, BoardSquare(4, D), BoardSquare(3, C)),
        Board(board, BoardSquare(4, D), BoardSquare(5, E)),
        Board(board, BoardSquare(4, D), BoardSquare(5, D)),
        Board(board, BoardSquare(4, D), BoardSquare(5, C)),
        Board(board, BoardSquare(4, D), BoardSquare(4, E)),
        Board(board, BoardSquare(4, D), BoardSquare(4, C)),
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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, E)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(7, E), BoardSquare(6, D)),
        Board(board, BoardSquare(7, E), BoardSquare(6, E)),
        Board(board, BoardSquare(7, E), BoardSquare(6, F)),
        Board(board, BoardSquare(7, E), BoardSquare(7, D)),
        Board(board, BoardSquare(7, E), BoardSquare(7, F)),

        Board(board, BoardSquare(7, E), BoardSquare(7, H)),
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
    std::vector<Board> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, E)};
    std::vector<Board> expectedValidMoves = {
        Board(board, BoardSquare(7, E), BoardSquare(6, D)),
        Board(board, BoardSquare(7, E), BoardSquare(6, E)),
        Board(board, BoardSquare(7, E), BoardSquare(6, F)),
        Board(board, BoardSquare(7, E), BoardSquare(7, D)),
        Board(board, BoardSquare(7, E), BoardSquare(7, F)),

        Board(board, BoardSquare(7, E), BoardSquare(7, A)),
    };
    validKingMoves(board, validMoves, whiteKings);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);    
}

TEST(MoveGenTest, moveGeneratorDefault) {
    Board board = Board();
    std::vector<Board> expectedValidMoves;
    for (int file = A; file <= H; file++) {
        expectedValidMoves.push_back(Board(board, BoardSquare(6, file), BoardSquare(5, file)));
        expectedValidMoves.push_back(Board(board, BoardSquare(6, file), BoardSquare(4, file)));
    }
    expectedValidMoves.push_back(Board(board, BoardSquare(7, B), BoardSquare(5, A)));
    expectedValidMoves.push_back(Board(board, BoardSquare(7, B), BoardSquare(5, C)));
    expectedValidMoves.push_back(Board(board, BoardSquare(7, G), BoardSquare(5, F)));
    expectedValidMoves.push_back(Board(board, BoardSquare(7, G), BoardSquare(5, H)));
    std::vector<Board> validMoves = moveGenerator(board);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);
}