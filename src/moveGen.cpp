#include "moveGen.hpp"
#include "attacks.hpp"

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace MOVEGEN {

    std::vector<BoardMove> moveGenerator(Board currBoard) {
        std::vector<BoardMove> listOfMoves;
        
        uint64_t kings   = currBoard.isWhiteTurn ? currBoard.pieceSets[WKing]   : currBoard.pieceSets[BKing]; 
        uint64_t pawns   = currBoard.isWhiteTurn ? currBoard.pieceSets[WPawn]   : currBoard.pieceSets[BPawn];
        uint64_t knights = currBoard.isWhiteTurn ? currBoard.pieceSets[WKnight] : currBoard.pieceSets[BKnight];
        uint64_t bishops = currBoard.isWhiteTurn ? currBoard.pieceSets[WBishop] : currBoard.pieceSets[BBishop];
        uint64_t rooks   = currBoard.isWhiteTurn ? currBoard.pieceSets[WRook]   : currBoard.pieceSets[BRook];
        uint64_t queens  = currBoard.isWhiteTurn ? currBoard.pieceSets[WQueen]  : currBoard.pieceSets[BQueen];

        validPawnMoves(currBoard, listOfMoves, pawns);
        validKnightMoves(currBoard, listOfMoves, knights);
        validBishopMoves(currBoard, listOfMoves, bishops);
        validRookMoves(currBoard, listOfMoves, rooks);
        validQueenMoves(currBoard, listOfMoves, queens);
        validKingMoves(currBoard, listOfMoves, kings);
        return listOfMoves;
    }

    void validPawnMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t pawns) {
        int promoteRank = currBoard.isWhiteTurn ? 0 : 7;
        pieceTypes allyKnight = currBoard.isWhiteTurn ? WKnight : BKnight;
        pieceTypes allyBishop = currBoard.isWhiteTurn ? WBishop : BBishop;
        pieceTypes allyRook = currBoard.isWhiteTurn ? WRook : BRook;
        pieceTypes allyQueen = currBoard.isWhiteTurn ? WQueen : BQueen;

        while (pawns) {
            BoardSquare pawn(popLeadingBit(pawns));
            std::vector<BoardSquare> pawnMoves;

            forwardPawnMoves(currBoard, pawnMoves, pawn);
            pawnCaptures(currBoard, pawnMoves, pawn, 1);
            pawnCaptures(currBoard, pawnMoves, pawn, -1);        
        
            for (BoardSquare target: pawnMoves) {
                if (!currBoard.isLegalMove(BoardMove(pawn, target))) {
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


    void forwardPawnMoves(Board& currBoard, std::vector<BoardSquare>& pawnMoves, BoardSquare pawn) {
        int pawnDirection = currBoard.isWhiteTurn ? -1 : 1;
        int originRank = currBoard.isWhiteTurn ? 6 : 1;

        // any spaces forward
        if (currBoard.getPiece(pawn.rank + pawnDirection, pawn.file) == EmptyPiece) {
            pawnMoves.push_back(BoardSquare(pawn.rank + pawnDirection, pawn.file));
            // jump
            if (pawn.rank == originRank) {
                if (currBoard.getPiece(pawn.rank + pawnDirection * 2, pawn.file) == EmptyPiece) {
                    pawnMoves.push_back(BoardSquare(pawn.rank + pawnDirection * 2, pawn.file));
            }
        }
        }
    }

    void pawnCaptures(Board& currBoard, std::vector<BoardSquare>& pawnMoves, BoardSquare pawn, int fileDirection) {
        int pawnDirection = currBoard.isWhiteTurn ? -1 : 1;

        BoardSquare square = BoardSquare(pawn.rank + pawnDirection, pawn.file + fileDirection);
        // regular capture
        if (square.isValid() && !isFriendlyPiece(currBoard, square) && currBoard.getPiece(square) != EmptyPiece) {
            pawnMoves.push_back(square);
        }
        // en passant
        if (square == currBoard.pawnJumpedSquare) {
            pawnMoves.push_back(square);
        }
    }

    void validKnightMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t knights) {
        uint64_t allies = currBoard.isWhiteTurn ? currBoard.pieceSets[WHITE_PIECES] : currBoard.pieceSets[BLACK_PIECES];
        while (knights) {
            int square = popLeadingBit(knights);
            BoardSquare knight(square);
            uint64_t knightBitboard = 1ull << square;
            uint64_t knightMoves = knightSquares(knightBitboard) & ~allies;
            while (knightMoves) {
                int currSquare = popLeadingBit(knightMoves);
                BoardMove move(knight, BoardSquare(currSquare));
                if (currBoard.isLegalMove(move)) {
                    validMoves.push_back(move);
                }
            }

        }
    }

    void validBishopMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t bishops) {
        uint64_t allPieces = currBoard.pieceSets[WHITE_PIECES] | currBoard.pieceSets[BLACK_PIECES];
        uint64_t friendlyPieces = currBoard.isWhiteTurn ? currBoard.pieceSets[WHITE_PIECES] : currBoard.pieceSets[BLACK_PIECES];
        while (bishops) {
            int square = popLeadingBit(bishops);
            BoardSquare bishop(square);
            uint64_t bishopMoves = Attacks::bishopAttacks(square, allPieces) & ~friendlyPieces;
            while (bishopMoves) {
                int currSquare = popLeadingBit(bishopMoves);
                BoardMove move(bishop, BoardSquare(currSquare));
                if (currBoard.isLegalMove(move)) {
                    validMoves.push_back(move);
                }
            }
        }
    }

    void validRookMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t rooks) {
        uint64_t allPieces = currBoard.pieceSets[WHITE_PIECES] | currBoard.pieceSets[BLACK_PIECES];
        uint64_t friendlyPieces = currBoard.isWhiteTurn ? currBoard.pieceSets[WHITE_PIECES] : currBoard.pieceSets[BLACK_PIECES];
        while (rooks) {
            int square = popLeadingBit(rooks);
            BoardSquare rook(square);
            uint64_t rookMoves = Attacks::rookAttacks(square, allPieces) & ~friendlyPieces;
            while (rookMoves) {
                int currSquare = popLeadingBit(rookMoves);
                BoardMove move(rook, BoardSquare(currSquare));
                if (currBoard.isLegalMove(move)) {
                    validMoves.push_back(move);
                }
            }
        }
    }

    void validQueenMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t queens) {
        validBishopMoves(currBoard, validMoves, queens);
        validRookMoves(currBoard, validMoves, queens);
    }


    void validKingMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t kings) {
        int kingUnmovedRank = currBoard.isWhiteTurn ? 7 : 0;
        
        while (kings) {
            BoardSquare king(popLeadingBit(kings));
            int currRank = king.rank;
            fileVals currFile = king.file;
            std::vector<BoardSquare> kingMoves;
            std::vector<BoardSquare> potentialAdjacentMoves = {
                BoardSquare(currRank - 1, currFile - 1),
                BoardSquare(currRank - 1, currFile),
                BoardSquare(currRank - 1, currFile + 1),
                BoardSquare(currRank + 1, currFile - 1),
                BoardSquare(currRank + 1, currFile),
                BoardSquare(currRank + 1, currFile + 1),
                BoardSquare(currRank, currFile - 1),
                BoardSquare(currRank, currFile + 1),
            };
            std::vector<BoardSquare> potentialCastleMoves = {
                BoardSquare(currRank, currFile - 2),
                BoardSquare(currRank, currFile + 2),
            };
            // regular movements
            for (BoardSquare square: potentialAdjacentMoves) {
                if (square.isValid() && !isFriendlyPiece(currBoard, square)) {
                    kingMoves.push_back(square);
                }
            }
            // castling
            for (BoardSquare square: potentialCastleMoves) {
                // check for castling rights and validity
                if (currKingInAttack(currBoard.pieceSets, currBoard.isWhiteTurn)) {break;}
                if (!(currBoard.castlingRights & castleRightsBit(square, currBoard.isWhiteTurn))) {continue;}


                // prevent castling through squares attacked by enemies
                int kingFileDirection = square.file == G ? 1 : -1;
                BoardMove oneOver(king, BoardSquare(king.rank, king.file + kingFileDirection));
                if (!currBoard.isLegalMove(oneOver)) {
                    continue;
                }

                // check for pieces in between king and rook
                if (unblockedCastleRook(currBoard, king, kingFileDirection)) {
                    kingMoves.push_back(BoardSquare(kingUnmovedRank, square.file));
                }
            }

            for (BoardSquare target: kingMoves) {
                BoardMove move(king, target);
                if (!target.isValid()) {
                    continue;
                }
                if (currBoard.isLegalMove(move)) {
                    validMoves.push_back(move);
                }
            }

        }
    }

    bool unblockedCastleRook(Board& currBoard, BoardSquare king, int xDir) {
        int currFile = king.file + xDir;
        int targetFile = xDir == 1 ? H : A;
        pieceTypes allyRook = currBoard.isWhiteTurn ? WRook : BRook;

        while (currFile >= A && currFile <= H) {
            pieceTypes currPiece = currBoard.getPiece(king.rank, currFile);
            if (currPiece == allyRook) {
                break;
            }
            else if (currPiece != EmptyPiece) {
                return false;
            }
            currFile += xDir;
        }
        return currFile == targetFile;
    }

    bool isFriendlyPiece(Board& currBoard, BoardSquare targetSquare) {
        int target = currBoard.getPiece(targetSquare);
        if(currBoard.isWhiteTurn) {
            return target >= WKing && target <= WPawn;
        }
        else {
            return target >= BKing && target <= BPawn;
        }
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
} // namespace MOVEGEN