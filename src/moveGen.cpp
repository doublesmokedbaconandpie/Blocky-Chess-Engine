#include "moveGen.hpp"

#include <stdexcept>
#include <vector>

std::vector<Board> moveGenerator(Board currBoard) {
    std::vector<Board> listOfMoves;
    std::vector<BoardSquare> pawns, knights, bishops, rooks, queens, kings;

    // get squares where each piece is

    // see pieceTypes definition
    int offset = 0;
    if (!currBoard.isWhiteTurn) {
        offset = BKing - WKing;
    }

    for (int rank = 0; rank < 8; rank++) {
        for (const auto file : { fileVals::A, fileVals::H } ) {
            int currPiece = currBoard.getPiece(rank, file);
            BoardSquare currSquare = BoardSquare(rank, file);
            if (currPiece == WPawn + offset || WPawnJumped + offset ) {
                pawns.push_back(currSquare);
            }
            if (currPiece == WBishop + offset) {
                pawns.push_back(currSquare);
            }
        }
    }

    // add logic to find all piece types

    validPawnMoves(currBoard, listOfMoves, pawns);
    validKnightMoves(currBoard, listOfMoves, knights);
    validBishopMoves(currBoard, listOfMoves, bishops);
    validRookMoves(currBoard, listOfMoves, rooks);
    validQueenMoves(currBoard, listOfMoves, queens);
    validKingMoves(currBoard, listOfMoves, kings);
    return listOfMoves;
}

void validPawnMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& pawns) {
    int promoteRank = currBoard.isWhiteTurn ? 0 : 7;
    pieceTypes allyKnight = currBoard.isWhiteTurn ? WKnight : BKnight;
    pieceTypes allyBishop = currBoard.isWhiteTurn ? WBishop : BBishop;
    pieceTypes allyRook = currBoard.isWhiteTurn ? WRook : BRook;
    pieceTypes allyQueen = currBoard.isWhiteTurn ? WQueen : BQueen;

    for (BoardSquare pawn: pawns) {
        std::vector<BoardSquare> pawnMoves;
        int currRank = pawn.rank;
        fileVals currFile = pawn.file;

        forwardPawnMoves(currBoard, pawnMoves, pawn);
        pawnCaptures(currBoard, pawnMoves, pawn, 1);
        pawnCaptures(currBoard, pawnMoves, pawn, -1);        
    
        for (BoardSquare move: pawnMoves) {
            Board potentialBoard = Board(currBoard, pawn, move);
            if (potentialBoard.isIllegalPos) {
                continue;
            }
            if (move.rank == promoteRank) {
                validMoves.push_back(Board(currBoard, pawn, move, allyKnight));
                validMoves.push_back(Board(currBoard, pawn, move, allyBishop));
                validMoves.push_back(Board(currBoard, pawn, move, allyRook));
                validMoves.push_back(Board(currBoard, pawn, move, allyQueen));
            }
            else {
                validMoves.push_back(potentialBoard);
            }
        }
    }
}


void forwardPawnMoves(Board& currBoard, std::vector<BoardSquare>& pawnMoves, BoardSquare pawn) {
    int pawnDirection = currBoard.isWhiteTurn ? -1 : 1;
    int originRank = currBoard.isWhiteTurn ? 6 : 1;

    // space forward
    if (currBoard.getPiece(pawn.rank + pawnDirection, pawn.file) == EmptyPiece) {
        pawnMoves.push_back(BoardSquare(pawn.rank + pawnDirection, pawn.file));
    }
    // jump
    if (pawn.rank == originRank) {
        if (currBoard.getPiece(pawn.rank + pawnDirection * 2, pawn.file) == EmptyPiece) {
            pawnMoves.push_back(BoardSquare(pawn.rank + pawnDirection * 2, pawn.file));
        }
    }
}

void pawnCaptures(Board& currBoard, std::vector<BoardSquare>& pawnMoves, BoardSquare pawn, int fileDirection) {
    pieceTypes enemyPawnJumped = currBoard.isWhiteTurn ? WPawnJumped : BPawnJumped;
    int pawnDirection = currBoard.isWhiteTurn ? -1 : 1;

    // left capture and en passant
    BoardSquare square = BoardSquare(pawn.rank + pawnDirection, pawn.file - fileDirection);
    if (!isFriendlyPiece(currBoard, square) && currBoard.getPiece(square) != EmptyPiece && currBoard.getPiece(square) != nullPiece) {
        pawnMoves.push_back(square);
    }
    if (currBoard.getPiece(pawn.rank, pawn.file - fileDirection) == enemyPawnJumped) {
        pawnMoves.push_back(square);
    }
}

void validKnightMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& knights) {
    for (BoardSquare knight: knights) {
        int currRank = knight.rank;
        fileVals currFile = knight.file;
        std::vector<BoardSquare> knightMoves;
        std::vector<BoardSquare> potentialMoves = {
            BoardSquare(currRank - 2, currFile - 1),
            BoardSquare(currRank - 2, currFile + 1),
            BoardSquare(currRank + 2, currFile - 1),
            BoardSquare(currRank + 2, currFile + 1),
            BoardSquare(currRank - 1, currFile - 2),
            BoardSquare(currRank - 1, currFile + 2),
            BoardSquare(currRank + 1, currFile - 2),
            BoardSquare(currRank + 1, currFile + 2),
        };
        for (BoardSquare square: potentialMoves) {
            if (!isFriendlyPiece(currBoard, square) && currBoard.getPiece(square) != nullPiece) {
                knightMoves.push_back(square);
            }
        }
        for (BoardSquare move: knightMoves) {
            Board potentialBoard = Board(currBoard, knight, move);
            if (!potentialBoard.isIllegalPos) {
                validMoves.push_back(potentialBoard);
            }
        }

    }
}

void validBishopMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& bishops) {
    for (BoardSquare bishop: bishops) {
        std::vector<BoardSquare> bishopMoves;
        addMovesInDirection(currBoard, bishopMoves, bishop, 1, 1); // down right
        addMovesInDirection(currBoard, bishopMoves, bishop, 1, -1); // down left
        addMovesInDirection(currBoard, bishopMoves, bishop, -1, 1); // up right
        addMovesInDirection(currBoard, bishopMoves, bishop, -1, -1); // up left
        for (BoardSquare move: bishopMoves) {
            Board potentialBoard = Board(currBoard, bishop, move);
            if (!potentialBoard.isIllegalPos) {
                validMoves.push_back(potentialBoard);
            }
        }
    }
}

void validRookMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& rooks) {
    for (BoardSquare rook: rooks) {
        std::vector<BoardSquare> rookMoves;
        addMovesInDirection(currBoard, rookMoves, rook, 1, 0); // down
        addMovesInDirection(currBoard, rookMoves, rook, -1, 0); // up
        addMovesInDirection(currBoard, rookMoves, rook, 0, -1); // left
        addMovesInDirection(currBoard, rookMoves, rook, 0, 1); // right
        for (BoardSquare move: rookMoves) {
            Board potentialBoard = Board(currBoard, rook, move);
            if (!potentialBoard.isIllegalPos) {
                validMoves.push_back(potentialBoard);
            }
        }
    }
}

void validQueenMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& queens) {
    validBishopMoves(currBoard, validMoves, queens);
    validRookMoves(currBoard, validMoves, queens);
}


void validKingMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& kings) {
    // get valid king moves

    // get castling
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
