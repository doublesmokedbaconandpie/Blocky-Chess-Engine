
#include "board.hpp"
#include "types.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

// BoardSquare

BoardSquare::BoardSquare(std::string input) {
    if (input == "-") {
        *this = BoardSquare();
        return;
    }
    this->file = fileVals(input.at(0) - 'a');
    this->rank = 8 - int(input.at(1) - '1') - 1;
}

bool operator==(const BoardSquare& lhs, const BoardSquare& rhs) {
    return (lhs.rank == rhs.rank) && (lhs.file == rhs.file);
}

bool operator!=(const BoardSquare& lhs, const BoardSquare& rhs) {
    return (lhs.rank != rhs.rank) || (lhs.file != rhs.file);
}

bool operator<(const BoardSquare& lhs, const BoardSquare& rhs) {
    if (lhs.rank != rhs.rank) {
        return lhs.rank < rhs.rank;
    }
    if (lhs.file != rhs.file) {
        return lhs.file < rhs.file;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const BoardSquare& target) {
    if (target.rank < 0 || target.rank > 7 || target.file < A || target.file > H) {
        os << target.file << ' ' << target.rank << ' ';
        return os;    
    }
    std::vector<char> fileRep = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    std::vector<char> rankRep = {'8', '7', '6', '5', '4', '3', '2', '1'};
    os << fileRep.at(target.file) << rankRep.at(target.rank);
    return os;
}

// BoardMove

BoardMove::BoardMove(std::string input, bool isWhiteTurn) {
    pieceTypes allyQueen = isWhiteTurn ? WQueen : BQueen;
    pieceTypes allyBishop = isWhiteTurn ? WBishop : BBishop;
    pieceTypes allyKnight = isWhiteTurn ? WKnight : BKnight;
    pieceTypes allyRook = isWhiteTurn ? WRook : BRook;
    
    this->pos1 = BoardSquare(input.substr(0, 2));
    this->pos2 = BoardSquare(input.substr(2, 2));

    if (input.length() == 5) {
        switch (input.at(4)) {
            case 'q':
                this->promotionPiece = allyQueen;
                break;
            case 'n':
                this->promotionPiece = allyKnight;
                break;
            case 'b':
                this->promotionPiece = allyBishop;
                break;
            case 'r':
                this->promotionPiece = allyRook;
                break;
            default:
                this->promotionPiece = nullPiece;
        }
    }
}

std::string BoardMove::toStr() {
    char pp;
    switch (this->promotionPiece) {
        case WQueen:
        case BQueen:
            pp = 'q';
            break;
        case WBishop:
        case BBishop:
            pp = 'b';
            break;
        case WRook:
        case BRook:
            pp = 'r';
            break;
        case WKnight:
        case BKnight:
            pp = 'n';
            break;
        default:
            pp = ' ';
    }
    std::ostringstream move;
    move << this->pos1;
    move << this->pos2;
    move << pp;
    return move.str();
}

std::ostream& operator<<(std::ostream& os, const BoardMove& target) {
    os <<  "Pos1[" << target.pos1 << "] Pos2[" << target.pos2 << "] Promote: " << target.promotionPiece;
    return os;
}

bool operator==(const BoardMove& lhs, const BoardMove& rhs) {
    return (lhs.pos1 == rhs.pos1) && (lhs.pos2 == rhs.pos2) && (lhs.promotionPiece == rhs.promotionPiece);
}

bool operator<(const BoardMove& lhs, const BoardMove& rhs) {
    if (lhs.pos1 != rhs.pos1) {
        return lhs.pos1 < rhs.pos1;
    }
    if (lhs.pos2 != rhs.pos2) {
        return lhs.pos2 < rhs.pos2;
    }
    return false;
}

// Board

Board::Board() {
    this->board = {
        {BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook},
        {BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece},
        {WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn},
        {WRook, WKnight, WBishop, WQueen, WKing, WBishop, WKnight, WRook}
    };
    this->isWhiteTurn = true;
    this->fiftyMoveRule = 0;
    this->pawnJumpedSquare = BoardSquare();
    this->isIllegalPos = false;
    this->castlingRights = All_Castle;
}

Board::Board(std::vector<std::vector<pieceTypes>> board, bool isWhiteTurn, int fiftyMoveRule,  BoardSquare pawnJumpedSquare, bool isIllegalPos, castleRights castlingRights) {
    this->board = board;
    this->isWhiteTurn = isWhiteTurn;
    this-> fiftyMoveRule = fiftyMoveRule;
    this->pawnJumpedSquare = pawnJumpedSquare;
    this->isIllegalPos = isIllegalPos;
    this->castlingRights = castlingRights;
}

Board::Board(std::string fenStr) {
    std::string token; 
    std::istringstream fenStream(fenStr);

    std::unordered_map<char, pieceTypes> charToPiece = {
        {'P', WPawn}, {'N', WKnight}, {'B', WBishop}, {'R', WRook}, {'Q', WQueen}, {'K', WKing}, 
        {'p', BPawn}, {'n', BKnight}, {'b', BBishop}, {'r', BRook}, {'q', BQueen}, {'k', BKing}, 
    };

    for (int i = 0; i <= 7; i++) { // need a non-empty board to use setPiece()    
        this->board.push_back({EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece});
    }
    fenStream >> token;
    int rank = 0, file = A;
    for (char& iter: token) {
        if (iter == '/') {
            rank++;
            file = A;
        }
        else if (charToPiece.find(iter) != charToPiece.end()) {
            this->setPiece(rank, file, charToPiece[iter]);
            file += 1;
        }
        else  { // must be a digit
            file += int(iter - '0');
        }
    }

    fenStream >> token;
    this->isWhiteTurn = token == "w" ? true : false;

    fenStream >> token;
    this->castlingRights = noCastle;
    this->castlingRights ^= token.find('K') != std::string::npos ? W_OO : noCastle;
    this->castlingRights ^= token.find('Q') != std::string::npos ? W_OOO : noCastle;
    this->castlingRights ^= token.find('k') != std::string::npos ? B_OO : noCastle;
    this->castlingRights ^= token.find('q') != std::string::npos ? B_OOO : noCastle;

    fenStream >> token;
    this->pawnJumpedSquare = BoardSquare(token);

    fenStream >> token;
    this->fiftyMoveRule = stoi(token);

    this->isIllegalPos = false; // it is up to the UCI gui to not give illegal positions

    // Board doesn't use Fullmove counter
}

pieceTypes Board::getPiece(int rank, int file) const {
    if (rank < 0 || rank > 7 || file < A || file > H) {
        return nullPiece;
    }
    return this->board.at(rank).at(file);
}

pieceTypes Board::getPiece(BoardSquare square) const{
    return this->getPiece(square.rank, square.file);
}

bool Board::setPiece(int rank, int file, pieceTypes piece) {
    if (rank < 0 || rank > 7 || file < A || file > H) {
        return false;
    }
    this->board.at(rank).at(file) = piece;
    return true;
}

bool Board::setPiece(BoardSquare square, pieceTypes piece) {
    return this->setPiece(square.rank, square.file, piece);
}

bool operator==(const Board& lhs, const Board& rhs) {
    return  (lhs.board == rhs.board);
}

bool operator<(const Board& lhs, const Board& rhs) {
    for (int rank = 0; rank <= 7; rank++) {
        for (int file = A; file <= H; file++) {
            pieceTypes lhsPiece = lhs.getPiece(rank, file);
            pieceTypes rhsPiece = rhs.getPiece(rank, file);
            if (lhsPiece != rhsPiece) {
                return lhsPiece < rhsPiece;
            }
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Board& target) {
    for (auto i: target.board) {
        os << "[";
        for (auto j: i) {
            switch (j)
            {
            case WPawn:
                os << "WP";
                break;
            case WKnight:
                os << "WN";
                break;
            case WBishop:
                os << "WB";
                break;
            case WRook:
                os << "WR";
                break;
            case WQueen:
                os << "WQ";
                break;
            case WKing:
                os << "WK";
                break;
            case BPawn:
                os << "BP";
                break;
            case BKnight:
                os << "BN";
                break;
            case BBishop:
                os << "BB";
                break;
            case BRook:
                os << "BR";
                break;
            case BQueen:
                os << "BQ";
                break;
            case BKing:
                os << "BK";
                break;
            default:
                os << "  ";
            }
            os << ',';
        }
        os<< "],\n";
    }
    os << "\n";
    os << "castlingRights: " << target.castlingRights << "\n";
    os << "isIllegalPos: " << target.isIllegalPos << "\n";
    os << "isWhiteTurn: " << target.isWhiteTurn << "\n";
    os << "50MoveRule: " << target.fiftyMoveRule << "\n";
    return os;
}

castleRights castleRightsBit(BoardSquare finalKingPos) {
    if (finalKingPos == BoardSquare(7, G)) {
        return W_OO;
    }
    else if (finalKingPos == BoardSquare(7, C)) {
        return W_OOO;
    }
    else if (finalKingPos == BoardSquare(0, G)) {
        return B_OO;
    }
    else if (finalKingPos == BoardSquare(0, C)) {
        return B_OOO;        
    }
    else {
        return noCastle;
    }
}
