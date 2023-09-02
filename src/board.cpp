#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

#include "board.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "types.hpp"

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
            this->setPiece(rank, file, charToPiece.at(iter));
            file += 1;

            this->materialDifference += pieceValues[charToPiece.at(iter)];
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

    int emptyPiecesInRow = 0, piecesPlaced = 0;
    for (pieceTypes piece: this->board) {
        if (piece == EmptyPiece) {
            emptyPiecesInRow++;
        }
        else if (emptyPiecesInRow != 0) {
            fenStr.append(std::to_string(emptyPiecesInRow));
            fenStr.push_back(pieceToChar.at(piece));
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
    this->castlingRights == noCastle ? fenStr.append("-") : ""; 
    fenStr.push_back(' ');

    fenStr.append(this->pawnJumpedSquare.toStr());
    fenStr.push_back(' ');

    fenStr.append(std::to_string(this->fiftyMoveRule));
    fenStr.push_back(' ');

    fenStr.push_back('1'); // Board doesn't use Fullmove counter

    return fenStr;
}

bool notInRange(int var) {return var < 0 || var > 7;}
void Board::makeMove(BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece) {
    if (notInRange(pos1.rank) || notInRange(pos1.file) || notInRange(pos2.file) || notInRange(pos2.rank)) {
        this->isIllegalPos = true;
        return;
    }

    // allies haven't made a move yet
    pieceTypes allyKing = this->isWhiteTurn ? WKing : BKing;
    pieceTypes allyRook = this->isWhiteTurn ? WRook : BRook;
    pieceTypes allyPawn = this->isWhiteTurn ? WPawn : BPawn;
    pieceTypes enemyRook = this->isWhiteTurn ? BRook : WRook;
    int pawnJumpDirection = this->isWhiteTurn ? -2 : 2;
    int promotionRank = this->isWhiteTurn ? 0 : 7;
    
    pieceTypes originPiece = this->getPiece(pos1);
    pieceTypes targetPiece = this->getPiece(pos2);

    this->moveHistory.push_back(BoardState(
        BoardMove(pos1, pos2, promotionPiece),
        originPiece,
        targetPiece,
        this->castlingRights,
        this->pawnJumpedSquare,
        this->fiftyMoveRule,
        this->materialDifference
    ));

    BoardSquare oldPawnJumpedSquare = this->pawnJumpedSquare;

    this->fiftyMoveRule++;

    this->setPiece(pos1, EmptyPiece); // origin square should be cleared in all situations
    this->setPiece(pos2, originPiece); // pretty much all possible moves translates the original piece to pos 2

    // castling
    // doesn't check for emptiness between rook and king
    if (originPiece == allyKing && (this->castlingRights & castleRightsBit(pos2, this->isWhiteTurn))) {
        int kingFileDirection = pos2.file > pos1.file ? 1 : -1;
        fileVals rookFile = kingFileDirection == 1 ? H : A;
        this->setPiece(pos1.rank, pos1.file + kingFileDirection, allyRook);
        this->setPiece(pos1.rank, rookFile, EmptyPiece);
        this->castlingRights &= allyKing == WKing ? B_Castle : W_Castle;
    }
    else if (originPiece == allyKing) {
        this->castlingRights &= allyKing == WKing ? B_Castle : W_Castle;
    }
    // jumping pawn
    else if (originPiece == allyPawn && pos2.rank == pos1.rank + pawnJumpDirection) { 
        // doesn't check if pawn's original position is rank 2
        int behindDirection = this->isWhiteTurn ? 1 : -1;
        this->pawnJumpedSquare = BoardSquare(pos2.rank + behindDirection, pos2.file);
        this->fiftyMoveRule = 0;
    }
    // promoting pawn
    else if (originPiece == allyPawn && pos2.rank == promotionRank) {
        this->setPiece(pos2, promotionPiece);
        this->fiftyMoveRule = 0;

        //updates material score of the board on promotion
        if(this->isWhiteTurn) {
            this->materialDifference += pieceValues[promotionPiece] - 1;
        }
        else {
            this->materialDifference += pieceValues[promotionPiece] + 1;
        }
    }
    // en passant 
    else if (originPiece == allyPawn && pos2 == this->pawnJumpedSquare) {
        this->setPiece(pos1.rank, pos2.file, EmptyPiece);
        this->fiftyMoveRule = 0;

        if(this->isWhiteTurn)
            materialDifference++;
        else
            materialDifference--;

    }
    else {
        if (targetPiece != EmptyPiece || originPiece == allyPawn) {
            this->fiftyMoveRule = 0;
        }
    }

    // if either your allyRook is moved or an enemyRook is captured, modify castling rights
    if (originPiece == allyRook) {
        this->castlingRights &= pos1 == BoardSquare("h1") ? NOT_W_OO  : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("a1") ? NOT_W_OOO : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("h8") ? NOT_B_OO  : All_Castle;
        this->castlingRights &= pos1 == BoardSquare("a8") ? NOT_B_OOO : All_Castle;
    }
    if (targetPiece == enemyRook) {
        this->castlingRights &= pos2 == BoardSquare("h1") ? NOT_W_OO  : All_Castle;
        this->castlingRights &= pos2 == BoardSquare("a1") ? NOT_W_OOO : All_Castle;
        this->castlingRights &= pos2 == BoardSquare("h8") ? NOT_B_OO  : All_Castle;
        this->castlingRights &= pos2 == BoardSquare("a8") ? NOT_B_OOO : All_Castle;
    }

    this->materialDifference -= pieceValues[targetPiece]; //updates the material score of the board on capture
    this->pawnJumpedSquare = this->pawnJumpedSquare == oldPawnJumpedSquare ? BoardSquare() : this->pawnJumpedSquare;

    this->isIllegalPos = currKingInAttack(*this);
    // after finalizing move logic, now switch turns
    this->isWhiteTurn = !this->isWhiteTurn; 
}

void Board::makeMove(BoardMove move) {
    this->makeMove(move.pos1, move.pos2, move.promotionPiece);
}


void Board::undoMove() {
    if (this->moveHistory.size() == 0) {
        return;
    }
    BoardState prev = moveHistory.back();

    pieceTypes prevKing = this->isWhiteTurn ? BKing : WKing;
    pieceTypes prevRook = this->isWhiteTurn ? BRook : WRook;
    pieceTypes prevPawn = this->isWhiteTurn ? BPawn : WPawn;

    this->setPiece(prev.move.pos1, prev.originPiece);
    this->setPiece(prev.move.pos2, prev.targetPiece);

    // castling
    if (prev.originPiece == prevKing && (prev.castlingRights & castleRightsBit(prev.move.pos2, !this->isWhiteTurn)) ) {
        int kingFileDirection = prev.move.pos2.file > prev.move.pos1.file ? 1 : -1;
        fileVals rookFile = kingFileDirection == 1 ? H : A;
        this->setPiece(prev.move.pos1.rank, prev.move.pos1.file + kingFileDirection, EmptyPiece);
        this->setPiece(prev.move.pos1.rank, rookFile, prevRook);
    }
    // en passant
    else if (prev.originPiece == prevPawn && prev.move.pos2 == prev.pawnJumpedSquare) {
        pieceTypes prevJumpedPawn = prevPawn == BPawn ? WPawn : BPawn;
        this->setPiece(prev.move.pos1.rank, prev.move.pos2.file, prevJumpedPawn);
    }

    this->isWhiteTurn = !this->isWhiteTurn;
    this->castlingRights = prev.castlingRights;
    this->pawnJumpedSquare = prev.pawnJumpedSquare;
    this->fiftyMoveRule = prev.fiftyMoveRule;
    this->materialDifference = prev.materialDifference;
    this->isIllegalPos = false;

    this->moveHistory.pop_back();
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
    for (int rank = 0; rank <= 7; rank++) {
        os << "[";
        for (int file = A; file <= 7; file++) {
            os << pieceToChar.at(target.getPiece(rank, file));
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

castleRights castleRightsBit(BoardSquare finalKingPos, bool isWhiteTurn) {
    if (finalKingPos == BoardSquare(7, G) && isWhiteTurn) {
        return W_OO;
    }
    else if (finalKingPos == BoardSquare(7, C) && isWhiteTurn) {
        return W_OOO;
    }
    else if (finalKingPos == BoardSquare(0, G) && !isWhiteTurn) {
        return B_OO;
    }
    else if (finalKingPos == BoardSquare(0, C) && !isWhiteTurn) {
        return B_OOO;        
    }
    else {
        return noCastle;
    }
}

bool currKingInAttack(Board& board) {
    pieceTypes allyKing = board.isWhiteTurn ? WKing : BKing;
    int kingSquare = leadingBit(board.pieceSets[allyKing]);
    assert(kingSquare != -1);

    uint64_t allPieces = board.pieceSets[WHITE_PIECES] | board.pieceSets[BLACK_PIECES];

    uint64_t enemyKings   = board.isWhiteTurn ? board.pieceSets[BKing]   : board.pieceSets[WKing];
    uint64_t enemyQueens  = board.isWhiteTurn ? board.pieceSets[BQueen]  : board.pieceSets[WQueen];
    uint64_t enemyBishops = board.isWhiteTurn ? board.pieceSets[BBishop] : board.pieceSets[WBishop];
    uint64_t enemyRooks   = board.isWhiteTurn ? board.pieceSets[BRook]   : board.pieceSets[WRook];
    uint64_t enemyKnights = board.isWhiteTurn ? board.pieceSets[BKnight] : board.pieceSets[WKnight];
    uint64_t enemyPawns   = board.isWhiteTurn ? board.pieceSets[BPawn]   : board.pieceSets[WPawn];

    return diagAttackers(kingSquare, allPieces, enemyQueens | enemyBishops)
        || straightAttackers(kingSquare, allPieces, enemyQueens | enemyRooks)
        || knightAttackers(kingSquare, enemyKnights)
        || pawnAttackers(kingSquare, enemyPawns, board.isWhiteTurn)
        || kingAttackers(kingSquare, enemyKings);
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