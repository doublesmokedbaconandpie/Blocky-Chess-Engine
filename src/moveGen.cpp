#include "moveGen.hpp"
#include "attacks.hpp"

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace MoveGen {

std::vector<BoardMove> moveGenerator(Board board) {
    std::vector<BoardMove> listOfMoves;
    
    // get all required helper bitboards for fast move generation
    MoveGenInfo info;
    info.allPieces = board.pieceSets[WHITE_PIECES] | board.pieceSets[BLACK_PIECES];
    info.emptySquares = ~info.allPieces;
    info.notAllies  = board.isWhiteTurn ? ~board.pieceSets[WHITE_PIECES] : ~board.pieceSets[BLACK_PIECES]; 
    info.isWhiteTurn = board.isWhiteTurn;

    // helper bitboards for pawn generation 
    info.pawnEnemies = board.isWhiteTurn ? board.pieceSets[BLACK_PIECES] : board.pieceSets[WHITE_PIECES]; 
    info.pawnEnemies |= board.enPassSquare != BoardSquare() ? 1ull << board.enPassSquare.toSquare() : 0ull;
    info.pawnStartRank = board.isWhiteTurn ? RANK_2 : RANK_7;
    info.pawnJumpRank  = board.isWhiteTurn ? RANK_4 : RANK_5;

    // helper bitboards for king castling
    info.pieceSets = board.pieceSets;
    info.castlingRights = board.castlingRights;
    info.castlingRights &= board.isWhiteTurn ? W_Castle : B_Castle;
    
    // gather piece bitboards
    uint64_t kings   = board.isWhiteTurn ? board.pieceSets[WKing]   : board.pieceSets[BKing]; 
    uint64_t pawns   = board.isWhiteTurn ? board.pieceSets[WPawn]   : board.pieceSets[BPawn];
    uint64_t knights = board.isWhiteTurn ? board.pieceSets[WKnight] : board.pieceSets[BKnight];
    uint64_t bishops = board.isWhiteTurn ? board.pieceSets[WBishop] : board.pieceSets[BBishop];
    uint64_t rooks   = board.isWhiteTurn ? board.pieceSets[WRook]   : board.pieceSets[BRook];
    uint64_t queens  = board.isWhiteTurn ? board.pieceSets[WQueen]  : board.pieceSets[BQueen];

    // generate moves
    validPawnMoves(pawns, info, board, listOfMoves);
    validPieceMoves(knights, knightMoves, info, board, listOfMoves);
    validPieceMoves(bishops, bishopMoves, info, board, listOfMoves);
    validPieceMoves(rooks, rookMoves, info, board, listOfMoves);
    validPieceMoves(queens, bishopMoves, info, board, listOfMoves);
    validPieceMoves(queens, rookMoves, info, board, listOfMoves);
    validPieceMoves(kings, kingMoves, info, board, listOfMoves);

    return listOfMoves;
}

template<typename Func>
void validPieceMoves(uint64_t pieces, Func pieceMoves, MoveGenInfo& info, Board& board, std::vector<BoardMove>& validMoves) {
    while (pieces) {
        int square = popLsb(pieces);
        BoardSquare piece(square);

        uint64_t moves = pieceMoves(square, info);
        while (moves) {
            int currSquare = popLsb(moves);
            BoardMove move(piece, BoardSquare(currSquare));
            if (board.isLegalMove(move)) {
                validMoves.push_back(move);
            }
        }
    }
}

void validPawnMoves(uint64_t pawns, MoveGenInfo& info, Board& board, std::vector<BoardMove>& validMoves) {
    int promoteRank = board.isWhiteTurn ? 0 : 7;
    pieceTypes allyKnight = board.isWhiteTurn ? WKnight : BKnight;
    pieceTypes allyBishop = board.isWhiteTurn ? WBishop : BBishop;
    pieceTypes allyRook = board.isWhiteTurn ? WRook : BRook;
    pieceTypes allyQueen = board.isWhiteTurn ? WQueen : BQueen;

    while (pawns) {
        int square = popLsb(pawns);
        BoardSquare pawn(square);
        uint64_t moves = pawnMoves(square, info, board.isWhiteTurn);

        while (moves) {
            BoardSquare target(popLsb(moves));
            if (!board.isLegalMove(BoardMove(pawn, target))) {
                continue;
            } 
            if (target.rank == promoteRank) {
                validMoves.push_back(BoardMove(pawn, target, allyKnight));
                validMoves.push_back(BoardMove(pawn, target, allyBishop));
                validMoves.push_back(BoardMove(pawn, target, allyRook));
                validMoves.push_back(BoardMove(pawn, target, allyQueen));
            }
            else {
                validMoves.push_back(BoardMove(pawn, target));
            }
        }
    }
}

uint64_t knightMoves(int square, MoveGenInfo& info) {
    return Attacks::knightAttacks(square) & info.notAllies;
}

uint64_t bishopMoves(int square, MoveGenInfo& info) {
    return Attacks::bishopAttacks(square, info.allPieces) & info.notAllies;
}

uint64_t rookMoves(int square, MoveGenInfo& info) {
    return Attacks::rookAttacks(square, info.allPieces) & info.notAllies;
}

uint64_t kingMoves(int square, MoveGenInfo& info) {
    // regular moves
    uint64_t moves = Attacks::kingAttacks(square) & info.notAllies;
    
    // castling
    // test if the king is in check; castling is illegal in check
    if (!currKingInAttack(info.pieceSets, info.isWhiteTurn)) {
        int kingIndex = info.isWhiteTurn ? WKing : BKing;

        while (info.castlingRights) {
            int currRight = popLsb(info.castlingRights);

            // check for emptiness between rook and king
            if (rookPaths[currRight] & info.allPieces) {
                continue;
            }

            // check for king path being attacked
            uint64_t path = kingPaths[currRight];
            int currSquare = popLsb(path);
            info.pieceSets[kingIndex] = 1ull << currSquare;
            if (1ull << currSquare & info.allPieces || 
                currKingInAttack(info.pieceSets, info.isWhiteTurn)) {
                    continue;
            }

            // perform castle
            moves |= castleDestination[currRight];
        }
    }

    return moves;
}

uint64_t pawnMoves(int square, MoveGenInfo& bitboards, bool isWhiteTurn) {
    uint64_t moves, pawn = 1ull << square;
    uint64_t currFile = FILES_MASK[getFile(square)];
    // one space forward
    if (isWhiteTurn) {
        moves = (pawn >> 8) & bitboards.emptySquares;
    } else {
        moves = (pawn << 8) & bitboards.emptySquares;
    } 
    // two spaces forward
    if (pawn & bitboards.pawnStartRank && moves) {
        moves |= (currFile & bitboards.pawnJumpRank) & bitboards.emptySquares;
    }
    // captures
    moves |= Attacks::pawnAttacks(square, isWhiteTurn) & bitboards.pawnEnemies;
    return moves;
}

// perft is a method of determining correctness of move generators
// positions can be input and number of total leaf nodes determined
// the number determined can be compared to a table to established values from others
uint64_t perft(Board board, int depthLeft) {
    if (depthLeft == 0) {
        return 1;
    }
    uint64_t leafNodeCount = 0;
    std::vector<BoardMove> moves = moveGenerator(board);
    for (auto move: moves) {
        board.makeMove(move);
        uint64_t moveCount = perftHelper(board, depthLeft - 1);
        leafNodeCount += moveCount;
        std::cout << move << ": " << moveCount << std::endl; 
        board.undoMove();
    }
    return leafNodeCount;
}

uint64_t perftHelper(Board board, int depthLeft) {
    if (depthLeft == 0) {
        return 1;
    }
    uint64_t leafNodeCount = 0;
    std::vector<BoardMove> moves = moveGenerator(board);
    for (auto move: moves) {
        board.makeMove(move);
        leafNodeCount += perftHelper(board, depthLeft - 1);
        board.undoMove();
    }
    return leafNodeCount;
}

} // namespace MoveGen