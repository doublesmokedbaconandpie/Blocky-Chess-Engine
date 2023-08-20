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
            int currPiece = currBoard.getPiece(file, rank);
            BoardSquare currSquare = BoardSquare(file, rank);
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
            validSquares.push_back(BoardSquare(currFile - 1, currRank - 2));
        }
        //up right
        if(currRank - 2 >= 0 && currFile + 1 <= H) {
            validSquares.push_back(BoardSquare(currFile - 1, currRank - 2));
        }
        //down left
        if(currRank + 2 <= 7 && currFile - 1 >= A) {
            validSquares.push_back(BoardSquare(currFile - 1, currRank - 2));
        }
        //down right
        if(currRank + 2 <= 7 && currFile + 1 <= H) {
            validSquares.push_back(BoardSquare(currFile - 1, currRank - 2));
        }
        //left up
        if(currRank - 1 >= 0 && currFile - 2 >= A) {
            validSquares.push_back(BoardSquare(currFile - 1, currRank - 2));
        }
        //left down
        if(currRank + 1 <= 7 && currFile - 2 >= A) {
            validSquares.push_back(BoardSquare(currFile - 1, currRank - 2));
        }
        //right up 
        if(currRank - 1 >= 0 && currFile + 2 <= H) {
            validSquares.push_back(BoardSquare(currFile - 1, currRank - 2));
        }
        //right down
        if(currRank + 1 <= 7 && currFile + 2 <= H) {
            validSquares.push_back(BoardSquare(currFile - 1, currRank - 2));
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
    
}

void validQueenMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& queens) {
    // repeat the logic used for rooks and bishops

    validRookMoves(currBoard, validMoves, queens);
    validBishopMoves(currBoard, validMoves, queens);
}


void validKingMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& kings) {
    // get valid king moves

    // get castling
}

//checks if a square has a friendly piece, useful for determining valid moves
bool isFriendlyPiece(Board& currBoard, BoardSquare targetSquare) {
    int target = currBoard.board.at(targetSquare.rank).at(targetSquare.file); 
    //rank could be wrong depending on how indexing works, could be (7 - rank) instead 

    if(currBoard.isWhiteTurn) {
        return target >= WKing && target <= WRookUnmoved;
    }
    else {
        return target >= BKing && target <= BRookUnmoved;
    }
}

//checks if a square has an enemy piece (currently planned to use for pawn captures)
bool isEnemyPiece(Board& currBoard, BoardSquare targetSquare) {
    int target = currBoard.board.at(targetSquare.rank).at(targetSquare.file); 
    //rank could be wrong depending on how indexing works, could be (7 - rank) instead 

    if(!currBoard.isWhiteTurn) {
        return target >= WKing && target <= WRookUnmoved;
    }
    else {
        return target >= BKing && target <= BRookUnmoved;
    }
}