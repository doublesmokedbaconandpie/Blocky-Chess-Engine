#include <stdexcept>

#include "moveGen.hpp"

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

//TODO: Add logic for check
//
//

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
        std::vector<BoardSquare> validSquares;
        //up left
        if(currRank - 2 >= 0 && currFile - 1 >= A) {
            validSquares.push_back(BoardSquare(currRank - 1, currFile - 2));
        }
        //up right
        if(currRank - 2 >= 0 && currFile + 1 <= H) {
            validSquares.push_back(BoardSquare(currRank - 1, currFile - 2));
        }
        //down left
        if(currRank + 2 <= 7 && currFile - 1 >= A) {
            validSquares.push_back(BoardSquare(currRank - 1, currFile - 2));
        }
        //down right
        if(currRank + 2 <= 7 && currFile + 1 <= H) {
            validSquares.push_back(BoardSquare(currRank - 1, currFile - 2));
        }
        //left up
        if(currRank - 1 >= 0 && currFile - 2 >= A) {
            validSquares.push_back(BoardSquare(currRank - 1, currFile - 2));
        }
        //left down
        if(currRank + 1 <= 7 && currFile - 2 >= A) {
            validSquares.push_back(BoardSquare(currRank - 1, currFile - 2));
        }
        //right up 
        if(currRank - 1 >= 0 && currFile + 2 <= H) {
            validSquares.push_back(BoardSquare(currRank - 1, currFile - 2));
        }
        //right down
        if(currRank + 1 <= 7 && currFile + 2 <= H) {
            validSquares.push_back(BoardSquare(currRank - 1, currFile - 2));
        }

        for(BoardSquare destination: validSquares) {
            Board move = Board(currBoard, knight, destination);

            if(/* see if the board doesn't puts the ally king in check; placeholder true */ true) {
                validMoves.push_back(move);
            }
        }

    }
}

void validBishopMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& bishops) {
    
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
    // repeat the logic used for rooks and bishops
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