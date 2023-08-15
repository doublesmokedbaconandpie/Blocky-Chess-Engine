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

    // check 2nd rank for move two spaces instead of 1

    // check 5th rank for en passant

    // check 7th rank for promotions

    // get pawn moves elsewhere (ranks 3, 4, 6)

}

void validKnightMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& knights) {
    for(BoardSquare knight: knights) {
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
            if (!isFriendlyPiece(currBoard, square) || currBoard.getPiece(square) == EmptyPiece) {
                knightMoves.push_back(square);
            }
        }
        std::cout << "LSAKDJFLKDSFJLKSDFJLKSDFJ" << potentialMoves.size() << std::endl;
        for(BoardSquare move: knightMoves) {
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