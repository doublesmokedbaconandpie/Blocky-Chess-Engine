#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "board.hpp"
#include "bitboard.hpp"
#include "types.hpp"

// BoardSquare

BoardSquare::BoardSquare(std::string input) {
    if (input == "-") {
        *this = BoardSquare();
        return;
    }
    this->file = fileVals(input.at(0) - 'a');
    this->rank = 8 - int(input.at(1) - '1') - 1;
}

int BoardSquare::toSquare() {
    return this->rank * 8 + this->file;
}

std::string BoardSquare::toStr() {
    std::ostringstream move;
    move << *this;
    return move.str();
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
    if (target == BoardSquare()) {
        os << "-";
        return os;
    }
    if (target.rank < 0 || target.rank > 7 || target.file < A || target.file > H) {
        os << target.file << ' ' << target.rank << ' ';
        return os;    
    }
    std::array<char, 8> fileRep = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    std::array<char, 8> rankRep = {'8', '7', '6', '5', '4', '3', '2', '1'};
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
        BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook,
        BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece, EmptyPiece,
        WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn,
        WRook, WKnight, WBishop, WQueen, WKing, WBishop, WKnight, WRook
    };
    this->isWhiteTurn = true;
    this->fiftyMoveRule = 0;
    this->pawnJumpedSquare = BoardSquare();
    this->isIllegalPos = false;
    this->castlingRights = All_Castle;
    this->materialDifference = 0;

    this->pieceSets[WKing]   = 0x1000000000000000ull;
    this->pieceSets[WQueen]  = 0x0800000000000000ull;
    this->pieceSets[WBishop] = 0x2400000000000000ull;
    this->pieceSets[WKnight] = 0x4200000000000000ull;
    this->pieceSets[WRook]   = 0x8100000000000000ull;
    this->pieceSets[WPawn]   = 0x00FF000000000000ull;

    this->pieceSets[BKing]   = 0x0000000000000010ull;
    this->pieceSets[BQueen]  = 0x0000000000000008ull;
    this->pieceSets[BBishop] = 0x0000000000000024ull;
    this->pieceSets[BKnight] = 0x0000000000000042ull;
    this->pieceSets[BRook]   = 0x0000000000000081ull;
    this->pieceSets[BPawn]   = 0x000000000000FF00ull;

    this->pieceSets[WHITE_PIECES] = 0xFFFF000000000000ull;
    this->pieceSets[BLACK_PIECES] = 0x000000000000FFFFull;
}

Board::Board(std::array<pieceTypes, BOARD_SIZE> a_board, bool a_isWhiteTurn, 
            int a_fiftyMoveRule, BoardSquare a_pawnJumpedSquare, 
            bool a_isIllegalPos, castleRights a_castlingRights, int a_materialDifference) {
    this->board = a_board;
    this->isWhiteTurn = a_isWhiteTurn;
    this->fiftyMoveRule = a_fiftyMoveRule;
    this->pawnJumpedSquare = a_pawnJumpedSquare;
    this->isIllegalPos = a_isIllegalPos;
    this->castlingRights = a_castlingRights;
    this->materialDifference = a_materialDifference;

    for (int i = WKing; i < WHITE_PIECES; i++) {
        this->pieceSets[i] = makeBitboardFromArray(this->board, i);
        if (i < BKing) {
            this->pieceSets[WHITE_PIECES] |= makeBitboardFromArray(this->board, i);
        }
        else {
            this->pieceSets[BLACK_PIECES] |= makeBitboardFromArray(this->board, i);
        }
    }
}

Board::Board(std::string fenStr) {
    this->materialDifference = 0;
    std::string token; 
    std::istringstream fenStream(fenStr);

    std::unordered_map<char, pieceTypes> charToPiece = {
        {'P', WPawn}, {'N', WKnight}, {'B', WBishop}, {'R', WRook}, {'Q', WQueen}, {'K', WKing}, 
        {'p', BPawn}, {'n', BKnight}, {'b', BBishop}, {'r', BRook}, {'q', BQueen}, {'k', BKing}, 
    };

    fenStream >> token;
    int rank = 0, file = A;
    for (char& iter: token) {
        if (iter == '/') {
            rank++;
            file = A;
        }
        else if (isdigit(iter))  { 
            file += int(iter - '0');
        }
        else { // must be a piece character
            this->setPiece(rank, file, charToPiece[iter]);
            file += 1;

            this->materialDifference += pieceValues.at(charToPiece[iter]);
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

std::string Board::toFen() {
    std::string fenStr; 

    std::unordered_map<pieceTypes, char> pieceToChar = {
        {WPawn, 'P'}, {WKnight, 'N'}, {WBishop, 'B'}, {WRook, 'R'}, {WQueen, 'Q'}, {WKing, 'K'}, 
        {BPawn, 'p'}, {BKnight, 'n'}, {BBishop, 'b'}, {BRook, 'r'}, {BQueen, 'q'}, {BKing, 'k'}, 
    };

    int emptyPiecesInRow = 0, piecesPlaced = 0;
    for (pieceTypes piece: this->board) {
        if (piece == EmptyPiece) {
            emptyPiecesInRow++;
        }
        else if (emptyPiecesInRow != 0) {
            fenStr.append(std::to_string(emptyPiecesInRow));
            fenStr.push_back(pieceToChar[piece]);
            emptyPiecesInRow = 0;
        }
        else {
            fenStr.push_back(pieceToChar.at(piece));
        }
        piecesPlaced++;
        
        if (piecesPlaced % 8 == 0) {
            emptyPiecesInRow ? fenStr.append(std::to_string(emptyPiecesInRow)) : ""; 
            fenStr.push_back('/');
            emptyPiecesInRow = 0;
        }
    }
    fenStr.pop_back(); // get rid of final '/'
    fenStr.push_back(' ');

    this->isWhiteTurn ? fenStr.append("w") : fenStr.append("b"); 
    fenStr.push_back(' ');

    this->castlingRights & W_OO  ? fenStr.append("K") : ""; 
    this->castlingRights & W_OOO ? fenStr.append("Q") : ""; 
    this->castlingRights & B_OO  ? fenStr.append("k") : ""; 
    this->castlingRights & B_OOO ? fenStr.append("q") : ""; 
    fenStr.push_back(' ');

    fenStr.append(this->pawnJumpedSquare.toStr());
    fenStr.push_back(' ');

    fenStr.append(std::to_string(this->fiftyMoveRule));
    fenStr.push_back(' ');

    fenStr.push_back('1'); // Board doesn't use Fullmove counter

    return fenStr;
}

pieceTypes Board::getPiece(int rank, int file) const {
    if (rank < 0 || rank > 7 || file < A || file > H) {
        return nullPiece;
    }
    return this->board[rank * 8 + file];
}

pieceTypes Board::getPiece(BoardSquare square) const{
    return this->getPiece(square.rank, square.file);
}

void Board::setPiece(int rank, int file, pieceTypes currPiece) {
    int square = rank * 8 + file;
    uint64_t setSquare = (1ull << square);
    uint64_t clearSquare = ALL_SQUARES ^ setSquare;

    pieceTypes originPiece = this->getPiece(rank, file);
    this->board[square] = currPiece;
    
    if (originPiece != nullPiece && originPiece != EmptyPiece) {
        pieceTypes originColor = originPiece < BKing ? WHITE_PIECES : BLACK_PIECES;
        this->pieceSets[originColor] &= clearSquare;
        this->pieceSets[originPiece] &= clearSquare;
    }
    if (currPiece != EmptyPiece) {
        pieceTypes currColor = currPiece < BKing ? WHITE_PIECES : BLACK_PIECES;
        this->pieceSets[currColor] ^= setSquare;
        this->pieceSets[currPiece] ^= setSquare;
    }
}

void Board::setPiece(BoardSquare square, pieceTypes currPiece) {
    this->setPiece(square.rank, square.file, currPiece);
}

bool operator==(const Board& lhs, const Board& rhs) {
    return  (lhs.board == rhs.board) && (lhs.pieceSets == rhs.pieceSets);
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
    std::unordered_map<pieceTypes, std::string> pieceToStr = {
        {WPawn, "WP"}, {WKnight, "WN"}, {WBishop, "WB"}, {WRook, "WR"}, {WQueen, "WQ"}, {WKing, "WK"}, 
        {BPawn, "BP"}, {BKnight, "BN"}, {BBishop, "BB"}, {BRook, "BR"}, {BQueen, "BQ"}, {BKing, "BK"}, 
        {EmptyPiece, "  "}
    };
    for (int rank = 0; rank <= 7; rank++) {
        os << "[";
        for (int file = A; file <= 7; file++) {
            os << pieceToStr[target.getPiece(rank, file)];
            os << ',';
        }
        os << "],\n";
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

uint64_t makeBitboardFromArray(std::array<pieceTypes, BOARD_SIZE> board, int target) {
    uint64_t result = 0ull;
    for (size_t i = 0; i < board.size(); i++) {
        if (board.at(i) == target) {
            result |= (1ull << i);
        }
    }
    return result;
}