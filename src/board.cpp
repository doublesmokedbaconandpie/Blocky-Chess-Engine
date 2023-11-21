#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "board.hpp"
#include "move.hpp"
#include "attacks.hpp"
#include "bitboard.hpp"
#include "zobrist.hpp"
#include "eval.hpp" 
#include "types.hpp"

Board::Board(std::string fenStr) {
    std::string token;
    std::istringstream fenStream(fenStr);

    this->zobristKey = 0;
    this->zobristKeyHistory = {0}; // required for setPiece

    std::fill(this->board.begin(), this->board.end(), EmptyPiece);
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
            pieceTypes currPiece = charToPiece.at(iter); 
            this->setPiece(rank, file, currPiece);
            file += 1;
        }
    }

    for(uint8_t i = 0; i < BOARD_SIZE; i++) {
        pieceTypes currPiece = board[i];
        if (currPiece != EmptyPiece) {
            this->eval.opScore += Eval::getPlacementScoreOp(i / 8, i % 8, currPiece);
            this->eval.egScore += Eval::getPlacementScoreEg(i / 8, i % 8, currPiece);
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
    this->enPassSquare = BoardSquare(token);

    fenStream >> token;
    this->fiftyMoveRule = stoi(token);

    this->initZobristKey();
    // Board doesn't use Fullmove counter
}

// For debugging
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

    fenStr.append(this->enPassSquare.toStr());
    fenStr.push_back(' ');

    fenStr.append(std::to_string(this->fiftyMoveRule));
    fenStr.push_back(' ');

    fenStr.push_back('1'); // Board doesn't use Fullmove counter

    return fenStr;
}

// assumes no move history
void Board::initZobristKey() {
    // castling
    for (size_t i = 0; i < 4; i++) {
        if (this->castlingRights & c_u64(1) << i) {
            this->zobristKey ^= Zobrist::castlingKeys[i];
        }
    }
    // en passant
    if (this->enPassSquare != BoardSquare()) {
        this->zobristKey ^= Zobrist::enPassKeys[this->enPassSquare.file];
    }
    // color to move
    if (!this->isWhiteTurn) {
        this->zobristKey ^= Zobrist::isBlackKey;
    }
    this->zobristKeyHistory = {this->zobristKey}; // synchronize history and current key
}

// makeMove will not check if the move is invalid
void Board::makeMove(BoardSquare pos1, BoardSquare pos2, pieceTypes promotionPiece) {
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
        this->enPassSquare,
        this->fiftyMoveRule
    ));

    BoardSquare oldPawnJumpedSquare = this->enPassSquare;
    castleRights oldCastlingRights = this->castlingRights;

    this->setPiece(pos1, EmptyPiece); // origin square should be cleared in all situations
    this->setPiece(pos2, originPiece); // pretty much all possible moves translates the original piece to pos 2

    // castling
    // doesn't check for emptiness between rook and king
    if (originPiece == allyKing && (this->castlingRights & castleRightsBit(pos2, this->isWhiteTurn))) {
        int kingFileDirection = pos2.file > pos1.file ? 1 : -1;
        fileVals rookFile = kingFileDirection == 1 ? H : A;
        this->setPiece(pos1.rank, pos1.file + kingFileDirection, allyRook);
        this->setPiece(pos1.rank, rookFile, EmptyPiece);
        this->castlingRights &= this->isWhiteTurn ? B_Castle : W_Castle;
    }
    else if (originPiece == allyKing) {
        this->castlingRights &= this->isWhiteTurn ? B_Castle : W_Castle;
    }
    // jumping pawn
    else if (originPiece == allyPawn && pos2.rank == pos1.rank + pawnJumpDirection) { 
        // doesn't check if pawn's original position is rank 2
        int behindDirection = this->isWhiteTurn ? 1 : -1;
        this->enPassSquare = BoardSquare(pos2.rank + behindDirection, pos2.file);
        this->zobristKey ^= Zobrist::enPassKeys[pos2.file];
    }
    // promoting pawn
    else if (originPiece == allyPawn && pos2.rank == promotionRank) {
        this->setPiece(pos2, promotionPiece);
    }
    // en passant 
    else if (originPiece == allyPawn && pos2 == this->enPassSquare) {
        this->setPiece(pos1.rank, pos2.file, EmptyPiece);
        this->enPassSquare = BoardSquare();
    }

    // reset fifty move rule on captures or pawn moves
    if (targetPiece != EmptyPiece || originPiece == allyPawn) {
        this->fiftyMoveRule = 0;
    }
    else {
        this->fiftyMoveRule++;
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

    // update zobrist key for changed castling rights; castling rights can only decrease in chess
    for (int i = 0; i < 4; i++) {
        int mask = c_u64(1) << i;
        if ((oldCastlingRights & mask) && !(this->castlingRights & mask)) {
            this->zobristKey ^= Zobrist::castlingKeys[i];
        }
    }

    // handle old en passant square 
    if (this->enPassSquare == oldPawnJumpedSquare) {
        this->enPassSquare = BoardSquare(); 
    }
    if (oldPawnJumpedSquare != BoardSquare()) {
        this->zobristKey ^= Zobrist::enPassKeys[oldPawnJumpedSquare.file];
    }	
    
    // after finalizing move logic, now switch turns
    this->isWhiteTurn = !this->isWhiteTurn; 
    this->zobristKey ^= Zobrist::isBlackKey;
    this->age++;

    // update history to include curr key
    this->zobristKeyHistory.push_back(this->zobristKey);
}

// makeMove will not check if the move is invalid
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
    else if (prev.originPiece == prevPawn && prev.move.pos2 == prev.enPassSquare) {
        pieceTypes prevJumpedPawn = prevPawn == BPawn ? WPawn : BPawn;
        this->setPiece(prev.move.pos1.rank, prev.move.pos2.file, prevJumpedPawn);
    }

    this->isWhiteTurn = !this->isWhiteTurn;
    this->castlingRights = prev.castlingRights;
    this->enPassSquare = prev.enPassSquare;
    this->fiftyMoveRule = prev.fiftyMoveRule;
    this->age--;

    this->moveHistory.pop_back();
    this->zobristKeyHistory.pop_back();
    this->zobristKey = zobristKeyHistory.back();
}

bool Board::moveIsCapture(BoardMove move) {
    if(this->getPiece(move.pos1) % 6 == WPawn && this->enPassSquare == move.pos2)
        return true;
    return this->getPiece(move.pos2) != EmptyPiece;
}

// getPiece is not responsible for bounds checking
pieceTypes Board::getPiece(int rank, int file) const {
    return this->board[rank * 8 + file];
}

// getPiece is not responsible for bounds checking
pieceTypes Board::getPiece(BoardSquare square) const{
    return this->getPiece(square.rank, square.file);
}

// handles board, pieceSets, and zobristKey (not including en passant and castling)
void Board::setPiece(int rank, int file, pieceTypes currPiece) {
    int square = rank * 8 + file;
    uint64_t setSquare = (c_u64(1) << square);
    uint64_t clearSquare = ALL_SQUARES ^ setSquare;

    pieceTypes originPiece = this->getPiece(rank, file);
    this->board[square] = currPiece;
    
    if (originPiece != EmptyPiece) {
        pieceTypes originColor = originPiece < BKing ? WHITE_PIECES : BLACK_PIECES;
        this->pieceSets[originColor] &= clearSquare;
        this->pieceSets[originPiece] &= clearSquare;
        this->zobristKey ^= Zobrist::pieceKeys[originPiece][square];
        this->eval.removePiece(rank, file, originPiece);
    }
    if (currPiece != EmptyPiece) {
        pieceTypes currColor = currPiece < BKing ? WHITE_PIECES : BLACK_PIECES;
        this->pieceSets[currColor] ^= setSquare;
        this->pieceSets[currPiece] ^= setSquare;
        this->zobristKey ^= Zobrist::pieceKeys[currPiece][square];
        this->eval.addPiece(rank, file, currPiece);
    }
}

void Board::setPiece(BoardSquare square, pieceTypes currPiece) {
    this->setPiece(square.rank, square.file, currPiece);
}

bool Board::isLegalMove(const BoardMove move) const {
    // This is a bitboard implementation to check whether a move leaves the ally king under attack
    // The current move generation already checks whether castling is even valid 
    // or squares unblocked so only the king final position needs to be checked

    assert(move.isValid());

    std::array<uint64_t, NUM_BITBOARDS> tmpPieceSets = this->pieceSets;

    pieceTypes originColor = this->isWhiteTurn ? WHITE_PIECES : BLACK_PIECES;
    uint64_t originSquare = (c_u64(1) << move.pos1.toSquare());
    pieceTypes originPiece = this->getPiece(move.pos1);
    pieceTypes allyPawn = this->isWhiteTurn ? WPawn : BPawn;
    pieceTypes enemyPawn = this->isWhiteTurn ? BPawn : WPawn;

    // account for captures
    uint64_t targetSquare = c_u64(1) << move.pos2.toSquare();
    pieceTypes targetPiece;
    pieceTypes targetColor = this->isWhiteTurn ? BLACK_PIECES : WHITE_PIECES;
    
    // en passant
    if (originPiece == allyPawn && move.pos2 == this->enPassSquare) {
        targetPiece = EmptyPiece;
        int dir = this->isWhiteTurn ? 8 : -8;
        uint64_t enemyPawnSquare = c_u64(1) << (move.pos2.toSquare() + dir);
        tmpPieceSets[targetColor] ^= enemyPawnSquare;
        tmpPieceSets[enemyPawn] ^= enemyPawnSquare;
    } else {
        targetPiece = this->getPiece(move.pos2);
    }
    // move ally piece 
    tmpPieceSets[originColor] ^= originSquare;
    tmpPieceSets[originPiece] ^= originSquare;
    tmpPieceSets[originColor] ^= targetSquare;
    tmpPieceSets[originPiece] ^= targetSquare;

    // modify destination for non-empty square
    if (targetPiece != EmptyPiece) {
        tmpPieceSets[targetColor] ^= targetSquare;
        tmpPieceSets[targetPiece] ^= targetSquare;
    }
    
    return !currKingInAttack(tmpPieceSets, this->isWhiteTurn);
}
    
// positive return values means winning for the side to move, negative is opposite
int Board::evaluate() const {
    int rawEval = this->eval.getRawEval();
    return this->isWhiteTurn ? rawEval : rawEval * -1;
}

bool operator==(const Board& lhs, const Board& rhs) {
    return (lhs.board == rhs.board) && (lhs.pieceSets == rhs.pieceSets) && (lhs.zobristKeyHistory == rhs.zobristKeyHistory);
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
    os << "isWhiteTurn: " << target.isWhiteTurn << "\n";
    os << "50MoveRule: " << target.fiftyMoveRule << "\n";
    os << "ZobristKey: " << target.zobristKey << "\n";
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

bool currKingInAttack(const std::array<uint64_t, NUM_BITBOARDS>& pieceSets, bool isWhiteTurn) {
    pieceTypes allyKing = isWhiteTurn ? WKing : BKing;
    assert(pieceSets[allyKing]);
    int kingSquare = lsb(pieceSets[allyKing]);

    uint64_t allPieces = pieceSets[WHITE_PIECES] | pieceSets[BLACK_PIECES];

    uint64_t enemyKings   = isWhiteTurn ? pieceSets[BKing]   : pieceSets[WKing];
    uint64_t enemyQueens  = isWhiteTurn ? pieceSets[BQueen]  : pieceSets[WQueen];
    uint64_t enemyBishops = isWhiteTurn ? pieceSets[BBishop] : pieceSets[WBishop];
    uint64_t enemyRooks   = isWhiteTurn ? pieceSets[BRook]   : pieceSets[WRook];
    uint64_t enemyKnights = isWhiteTurn ? pieceSets[BKnight] : pieceSets[WKnight];
    uint64_t enemyPawns   = isWhiteTurn ? pieceSets[BPawn]   : pieceSets[WPawn];

    return Attacks::bishopAttacks(kingSquare, allPieces) & (enemyBishops | enemyQueens)
        || Attacks::rookAttacks(kingSquare, allPieces) & (enemyRooks | enemyQueens)
        || Attacks::pawnAttacks(kingSquare, isWhiteTurn) & enemyPawns
        || Attacks::knightAttacks(kingSquare) & enemyKnights
        || Attacks::kingAttacks(kingSquare) & enemyKings;
}
