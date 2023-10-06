#include "moveGen.hpp"

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
        
            for (BoardSquare move: pawnMoves) {
                currBoard.makeMove(pawn, move, allyKnight); // promotion piece is a placeholder
                if (currBoard.isIllegalPos) {
                    currBoard.undoMove();
                    continue;
                }
                if (move.rank == promoteRank) {
                    validMoves.push_back(BoardMove(pawn, move, allyKnight));
                    validMoves.push_back(BoardMove(pawn, move, allyBishop));
                    validMoves.push_back(BoardMove(pawn, move, allyRook));
                    validMoves.push_back(BoardMove(pawn, move, allyQueen));
                }
                else {
                    validMoves.push_back(BoardMove(pawn, move));
                }
                currBoard.undoMove();
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
            BoardSquare knight(popLeadingBit(knights));
            uint64_t knightBitboard = 1ull << knight.toSquare();
            uint64_t knightMoves = knightSquares(knightBitboard) & ~allies;
            while (knightMoves) {
                int currSquare = popLeadingBit(knightMoves);
                BoardSquare move(currSquare);
                currBoard.makeMove(knight, move);
                if (!currBoard.isIllegalPos) {
                    validMoves.push_back(BoardMove(knight, move));
                }
                currBoard.undoMove();
            }

        }
    }

    void validBishopMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t bishops) {
        while (bishops) {
            BoardSquare bishop(popLeadingBit(bishops));
            std::vector<BoardSquare> bishopMoves;
            addMovesInDirection(currBoard, bishopMoves, bishop, 1, 1); // down right
            addMovesInDirection(currBoard, bishopMoves, bishop, 1, -1); // down left
            addMovesInDirection(currBoard, bishopMoves, bishop, -1, 1); // up right
            addMovesInDirection(currBoard, bishopMoves, bishop, -1, -1); // up left
            for (BoardSquare move: bishopMoves) {
                currBoard.makeMove(bishop, move);
                if (!currBoard.isIllegalPos) {
                    validMoves.push_back(BoardMove(bishop, move));
                }
                currBoard.undoMove();
            }
        }
    }

    void validRookMoves(Board& currBoard, std::vector<BoardMove>& validMoves, uint64_t rooks) {
        while (rooks) {
            BoardSquare rook(popLeadingBit(rooks));
            std::vector<BoardSquare> rookMoves;
            addMovesInDirection(currBoard, rookMoves, rook, 1, 0); // down
            addMovesInDirection(currBoard, rookMoves, rook, -1, 0); // up
            addMovesInDirection(currBoard, rookMoves, rook, 0, -1); // left
            addMovesInDirection(currBoard, rookMoves, rook, 0, 1); // right
            for (BoardSquare move: rookMoves) {
                currBoard.makeMove(rook, move);
                if (!currBoard.isIllegalPos) {
                    validMoves.push_back(BoardMove(rook, move));
                }
                currBoard.undoMove();
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
                if (currKingInAttack(currBoard)) {break;}
                if (!(currBoard.castlingRights & castleRightsBit(square, currBoard.isWhiteTurn))) {continue;}
                
                // prevent castling through squares attacked by enemies
                int kingFileDirection = square.file == G ? 1 : -1;
                currBoard.makeMove(king, BoardSquare(king.rank, king.file + kingFileDirection));
                if (currBoard.isIllegalPos) {
                    currBoard.undoMove();
                    continue;
                } 
                currBoard.undoMove();

                // check for pieces in between king and rook
                if (unblockedCastleRook(currBoard, king, kingFileDirection)) {
                    kingMoves.push_back(BoardSquare(kingUnmovedRank, square.file));
                }
            }
            
            for (BoardSquare move: kingMoves) {
                currBoard.makeMove(king, move);
                if (!currBoard.isIllegalPos) {
                    validMoves.push_back(BoardMove(king, move));
                }
                currBoard.undoMove();
            }

        }
    }

    void addMovesInDirection(Board& currBoard, std::vector<BoardSquare>& movesVec, BoardSquare originSquare, int rankIncrement, int fileIncrement) {
        // static/non-moves will not be appended
        if (rankIncrement == 0 && fileIncrement == 0) {
            throw std::invalid_argument("rankIncrement or fileIncrement must not be 0");
        }
        
        pieceTypes currPiece;
        int currRank = originSquare.rank + rankIncrement;
        int currFile = originSquare.file + fileIncrement;

        while(currRank >= 0 && currRank <= 7 && currFile >= A && currFile <= H) {
            BoardSquare currSquare = BoardSquare(currRank, currFile);
            currPiece = currBoard.getPiece(currSquare);
            if (isFriendlyPiece(currBoard, currSquare)) {
                break;
            }
            movesVec.push_back(BoardSquare(currRank, currFile));
            if (currPiece != EmptyPiece) {
                break;
            }
            currRank += rankIncrement;
            currFile += fileIncrement;
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