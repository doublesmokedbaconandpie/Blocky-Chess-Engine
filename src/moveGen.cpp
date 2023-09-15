#include "moveGen.hpp"

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace MOVEGEN {

    std::vector<BoardMove> moveGenerator(Board currBoard) {
        std::vector<BoardMove> listOfMoves;
        std::vector<BoardSquare> pawns, knights, bishops, rooks, queens, kings;
        
        pieceTypes allyKing = currBoard.isWhiteTurn ? WKing : BKing;
        pieceTypes allyPawn = currBoard.isWhiteTurn ? WPawn : BPawn;
        pieceTypes allyKnight = currBoard.isWhiteTurn ? WKnight : BKnight;
        pieceTypes allyBishop = currBoard.isWhiteTurn ? WBishop : BBishop;
        pieceTypes allyRook = currBoard.isWhiteTurn ? WRook : BRook;
        pieceTypes allyQueen = currBoard.isWhiteTurn ? WQueen : BQueen;

        for (int rank = 0; rank < 8; rank++) {
            for (int file = A; file <= H; file++) {
                int currPiece = currBoard.getPiece(rank, file);
                BoardSquare currSquare = BoardSquare(rank, file);
                if (currPiece == allyKnight) {knights.push_back(currSquare);}
                if (currPiece == allyBishop) {bishops.push_back(currSquare);}
                if (currPiece == allyQueen) {queens.push_back(currSquare);}
                if (currPiece == allyPawn) {pawns.push_back(currSquare);}
                if (currPiece == allyRook) {rooks.push_back(currSquare);}
                if (currPiece == allyKing) {kings.push_back(currSquare);}
            }
        }

        validPawnMoves(currBoard, listOfMoves, pawns);
        validKnightMoves(currBoard, listOfMoves, knights);
        validBishopMoves(currBoard, listOfMoves, bishops);
        validRookMoves(currBoard, listOfMoves, rooks);
        validQueenMoves(currBoard, listOfMoves, queens);
        validKingMoves(currBoard, listOfMoves, kings);
        return listOfMoves;
    }

    void validPawnMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& pawns) {
        int promoteRank = currBoard.isWhiteTurn ? 0 : 7;
        pieceTypes allyKnight = currBoard.isWhiteTurn ? WKnight : BKnight;
        pieceTypes allyBishop = currBoard.isWhiteTurn ? WBishop : BBishop;
        pieceTypes allyRook = currBoard.isWhiteTurn ? WRook : BRook;
        pieceTypes allyQueen = currBoard.isWhiteTurn ? WQueen : BQueen;

        for (BoardSquare pawn: pawns) {
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

    void validKnightMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& knights) {
        uint64_t allies = currBoard.isWhiteTurn ? currBoard.pieceSets[WHITE_PIECES] : currBoard.pieceSets[BLACK_PIECES];
        for (BoardSquare knight: knights) { 
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

    void validBishopMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& bishops) {
        for (BoardSquare bishop: bishops) {
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

    void validRookMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& rooks) {
        for (BoardSquare rook: rooks) {
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

    void validQueenMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& queens) {
        validBishopMoves(currBoard, validMoves, queens);
        validRookMoves(currBoard, validMoves, queens);
    }


    void validKingMoves(Board& currBoard, std::vector<BoardMove>& validMoves, std::vector<BoardSquare>& kings) {
        pieceTypes allyRook = currBoard.isWhiteTurn ? WRook : BRook;
        int kingUnmovedRank = currBoard.isWhiteTurn ? 7 : 0;
        
        for (BoardSquare king: kings) {
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
                if (getPieceInDirection(currBoard, king, 0, kingFileDirection) == allyRook) {
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

    pieceTypes getPieceInDirection(Board& currBoard, BoardSquare originSquare, int rankIncrement, int fileIncrement) {
        if (rankIncrement == 0 && fileIncrement == 0) {
            throw std::invalid_argument("rankIncrement or fileIncrement must not be 0");
        }
        
        pieceTypes currPiece;
        int currRank = originSquare.rank + rankIncrement;
        int currFile = originSquare.file + fileIncrement;

        while(currRank >= 0 && currRank <= 7 && currFile >= A && currFile <= H) {
            currPiece = currBoard.getPiece(currRank, currFile);
            if (currPiece != EmptyPiece) {
                return currPiece;
            }
            currRank += rankIncrement;
            currFile += fileIncrement;
        }
        return nullPiece;
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
            leafNodeCount += perft(board, depthLeft - 1);
            board.undoMove();
        }
        return leafNodeCount;
    }
} // namespace MOVEGEN