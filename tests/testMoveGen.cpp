#include "board.hpp"
#include "moveGen.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

using namespace MOVEGEN;

TEST(MoveGenTest, isFriendlyPieceTrue1) {
    Board board;
    BoardSquare whiteSquare = BoardSquare(7, H);
    bool result = isFriendlyPiece(board, whiteSquare);
    ASSERT_EQ(result, true);
}

TEST(MoveGenTest, isFriendlyPieceTrue2) {
    Board board;
    board.isWhiteTurn = false;
    BoardSquare blackSquare = BoardSquare(1, H);
    bool result = isFriendlyPiece(board, blackSquare);
    ASSERT_EQ(result, true);
}

TEST(MoveGenTest, isFriendlyPieceFalse) {
    Board board;
    BoardSquare blackSquare = BoardSquare(1, H);
    bool result = isFriendlyPiece(board, blackSquare);
    ASSERT_EQ(result, false);
}

TEST(MoveGenTest, getPieceInDirectionStraight) {
    Board board;
    BoardSquare originSquare = BoardSquare(1, D); // black pawn
    pieceTypes down = getPieceInDirection(board, originSquare, 1, 0);
    pieceTypes up = getPieceInDirection(board, originSquare, -1, 0);
    pieceTypes right = getPieceInDirection(board, originSquare, 0, 1);
    pieceTypes left = getPieceInDirection(board, originSquare, 0, -1);
    EXPECT_EQ(down, WPawn);
    EXPECT_EQ(up, BQueen);
    EXPECT_EQ(right, BPawn);
    EXPECT_EQ(left, BPawn);
}

TEST(MoveGenTest, getPieceInDirectionDiag) {
    Board board;
    BoardSquare originSquare = BoardSquare(1, E); // black pawn
    pieceTypes topRight = getPieceInDirection(board, originSquare, -1, 1);
    pieceTypes topLeft = getPieceInDirection(board, originSquare, -1, -1);
    pieceTypes bottomLeft = getPieceInDirection(board, originSquare, 1, -1);
    pieceTypes bottomRight = getPieceInDirection(board, originSquare, 1, 1);
    EXPECT_EQ(topRight, BBishop);
    EXPECT_EQ(topLeft, BQueen);
    EXPECT_EQ(bottomLeft, nullPiece);
    EXPECT_EQ(bottomRight, nullPiece);
}

// there must be a king in each test to see for illegal moves

TEST(MoveGenTest, validPawnMovesCaptures) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        BPawn     , EmptyPiece, BPawn     , EmptyPiece, WQueen    , EmptyPiece, EmptyPiece, EmptyPiece,
        WKing     , WPawn     , EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board = Board(boardArr);
    
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
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WPawn     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board = Board(boardArr);
    
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
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, BKnight   , EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece,
        WKnight   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board = Board(boardArr);
    
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
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        WBishop   , WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , WRook     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BRook     ,
    };
    Board board = Board(boardArr, true);
    
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
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BBishop   , EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WBishop   , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WBishop   , WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board = Board(boardArr);
    
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
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WQueen    , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, WQueen    ,
    };
    Board board = Board(boardArr);
    
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
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, BPawn     , EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WKing     , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
    };
    Board board = Board(boardArr);
    
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
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, WBishop   , EmptyPiece, WKing     , EmptyPiece, EmptyPiece, WRook     ,
    };
    Board board = Board(boardArr);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, E)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(7, E), BoardSquare(6, D)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, E)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, F)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, D)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, F)),

        BoardMove(BoardSquare(7, E), BoardSquare(7, G)),
    };
    validKingMoves(board, validMoves, whiteKings);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);    
}

TEST(MoveGenTest, validKingMovesQueenCastle) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, EmptyPiece, EmptyPiece, WKing     , EmptyPiece, WBishop   , WRook     ,
    };
    Board board = Board(boardArr);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, E)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(7, E), BoardSquare(6, D)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, E)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, F)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, D)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, F)),

        BoardMove(BoardSquare(7, E), BoardSquare(7, C)),
    };
    validKingMoves(board, validMoves, whiteKings);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(validMoves, expectedValidMoves);    
}

TEST(MoveGenTest, validKingMovesInvalidCastle) {
    std::array<pieceTypes, BOARD_SIZE> boardArr = {
        EmptyPiece, EmptyPiece, EmptyPiece, BQueen    , EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WRook     , EmptyPiece, EmptyPiece, EmptyPiece, WKing     , EmptyPiece, WBishop   , WRook     ,
    };
    Board board = Board(boardArr);
    
    std::vector<BoardMove> validMoves;
    std::vector<BoardSquare> whiteKings = {BoardSquare(7, E)};
    std::vector<BoardMove> expectedValidMoves = {
        BoardMove(BoardSquare(7, E), BoardSquare(6, E)),
        BoardMove(BoardSquare(7, E), BoardSquare(6, F)),
        BoardMove(BoardSquare(7, E), BoardSquare(7, F)),
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
    Board board;
    board.makeMove(BoardSquare(6, E), BoardSquare(4, E));
    std::vector<BoardMove> expectedValidMoves;
    for (int file = A; file <= H; file++) {
        expectedValidMoves.push_back(BoardMove(BoardSquare(1, file), BoardSquare(2, file)));
        expectedValidMoves.push_back(BoardMove(BoardSquare(1, file), BoardSquare(3, file)));
    }
    expectedValidMoves.push_back(BoardMove(BoardSquare(0, B), BoardSquare(2, A)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(0, B), BoardSquare(2, C)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(0, G), BoardSquare(2, F)));
    expectedValidMoves.push_back(BoardMove(BoardSquare(0, G), BoardSquare(2, H)));
    std::vector<BoardMove> validMoves = moveGenerator(board);

    std::sort(validMoves.begin(), validMoves.end());
    std::sort(expectedValidMoves.begin(), expectedValidMoves.end());
    ASSERT_EQ(board.isWhiteTurn, false);
    ASSERT_EQ(validMoves, expectedValidMoves);
}