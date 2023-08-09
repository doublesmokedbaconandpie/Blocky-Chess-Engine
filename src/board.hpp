
#include <vector>

enum fileVals {nullFile = -1, A, B, C, D, E, F, G, H};
enum pieceTypes {WKing = 1, WQueen, WBishop, WKnight, WRook, WPawn, 
                WPawnJumped, WKingUnmoved, WRookUnmoved,
                BKing, BQueen, BBishop, BKnight, BRook, BPawn,
                BPawnJumped, BKingUnmoved, BRookUnmoved,};

struct BoardSquare {
    BoardSquare(): file(nullFile), rank(-1) {};
    BoardSquare(fileVals fileVal, int rankVal): file(fileVal), rank(rankVal) {};
    fileVals file;
    int rank;
};

struct Board {
    Board(); // begin default game
    Board(std::vector<std::vector<int>> board, bool isWhiteTurn = true, 
            int movesWithoutCapture = 0, bool pawnJumped = false, 
            BoardSquare pawnJumpedSquare = BoardSquare(), bool inCheck = false); // maybe useful for testing or chess960
    Board(Board originalBoard, BoardSquare pos1, BoardSquare pos2); // creates a new board for a move 

    std::vector<std::vector<int>> board;
    bool isWhiteTurn;
    int movesWithoutCapture; // 50 move rule
    bool inCheck;
    bool pawnJumped;
    BoardSquare pawnJumpedSquare;
};

std::vector<Board> moveGenerator(Board currBoard); // outputs board instead of board moves for future evaluation functions
void validPawnMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& pawns); // includes en passant
void validKnightMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& knights);
void validBishopMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& bishops);
void validRookMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& rooks);
void validQueenMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& queens);
void validKingMoves(Board& currBoard, std::vector<Board>& validMoves, std::vector<BoardSquare>& kings); // includes castling
// idk what to do about 3-fold repetition; maybe a hash table

