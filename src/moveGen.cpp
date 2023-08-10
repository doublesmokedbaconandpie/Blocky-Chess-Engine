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

void validPawnMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& pawns) {

    // check 2nd rank for move two spaces instead of 1

    // check 5th rank for en passant

    // check 7th rank for promotions

    // get pawn moves elsewhere (ranks 3, 4, 6)

}

void validKnightMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& knights) {
    // iterate through currBoard to find the knights
}

void validBishopMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& bishops) {
    // iterate through currBoard to find the bishops
}

void validRookMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& rooks) {
    for (BoardSquare rookSquare: rooks) {
        std::vector<BoardSquare> downMoves, upMoves, leftMoves, rightMoves;
        pieceTypes downPiece, upPiece, leftPiece, rightPiece;
        pieceTypes downLeftPiece, downRightPiece, upLeftPiece, upRightPiece; 
        downPiece, upPiece, leftPiece, rightPiece = EmptyPiece;
        pieceTypes rookType = currBoard.getPiece(rookSquare);
        
        // add all moves in corresponding directions, ignoring pins
        addMovesInDirection(currBoard, downMoves, rookSquare, 1, 0);
        addMovesInDirection(currBoard, upMoves, rookSquare, -1, 0);
        addMovesInDirection(currBoard, leftMoves, rookSquare, 0, -1);
        addMovesInDirection(currBoard, rightMoves, rookSquare, 0, 1);

        // check for any possible piece colissions in all directions
        if (!downMoves.empty()) {downPiece = currBoard.getPiece(downMoves.back());}
        if (!upMoves.empty()) {upPiece = currBoard.getPiece(upMoves.back());}
        if (!leftMoves.empty()) {leftPiece = currBoard.getPiece(leftMoves.back());}
        if (!rightMoves.empty()) {rightPiece = currBoard.getPiece(rightMoves.back());}
        downLeftPiece = checkPieceInDirection(currBoard, rookSquare, 1, -1);
        downRightPiece = checkPieceInDirection(currBoard, rookSquare, 1, 1);
        upLeftPiece = checkPieceInDirection(currBoard, rookSquare, -1, -1);
        upRightPiece = checkPieceInDirection(currBoard, rookSquare, -1, 1);

        // check pins
        bool diagPin = false;
        if (checkDiagPin(rookType, downLeftPiece, upRightPiece) ||
            checkDiagPin(rookType, upLeftPiece, downRightPiece)) {
            diagPin = true;
        }
        bool vertPin = checkStraightPin(rookType, downPiece, upPiece);
        bool horiPin = checkStraightPin(rookType, leftPiece, rightPiece);

        // prune valid moves based on pins
        if (diagPin) {
            return;
        }
        if (!vertPin) {
            validMoves.insert(validMoves.end(), leftMoves.begin(), leftMoves.end());
            validMoves.insert(validMoves.end(), rightMoves.begin(), rightMoves.end());
        }
        if (!horiPin) {
            validMoves.insert(validMoves.end(), upMoves.begin(), upMoves.end());
            validMoves.insert(validMoves.end(), downMoves.begin(), downMoves.end());
        }
    }
}

void validQueenMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& queens) {
    // iterate through currBoard to find the queens (could be more due to promotions)
    // repeat the logic used for rooks and bishops
}


void validKingMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& kings) {
    // get valid king moves

    // get castling
}

