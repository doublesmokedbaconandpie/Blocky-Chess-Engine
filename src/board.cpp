/*
* Blocky, a UCI chess engine
* Copyright (C) 2023-2024, Kevin Nguyen
*
* Blocky is free software; you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
*
* Blocky is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along with this program; 
* if not, see <https://www.gnu.org/licenses>.
*/

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

#include "utils/types.hpp"

Board::Board(std::string fenStr) {
    std::string token;
    std::istringstream fenStream(fenStr);

    this->zobristKey = 0;
    this->zobristKeyHistory = {0}; // required for setPiece

    std::fill(this->board.begin(), this->board.end(), EmptyPiece);
    fenStream >> token;
    int rank = 0, file = 0;
    for (const char& iter: token) {
        if (iter == '/') {
            rank++;
            file = 0;
        }
        else if (isdigit(iter))  { 
            file += static_cast<int>(iter - '0');
        }
        else { // must be a piece character
            pieceTypes currPiece = charToPiece.at(iter); 
            this->setPiece(toSquare(rank, file), currPiece);
            file += 1;
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
    this->enPassSquare = toSquare(token);

    fenStream >> token;
    this->fiftyMoveRule = stoi(token);

    this->initZobristKey();
    // Board doesn't use Fullmove counter
}

// For debugging
std::string Board::toFen() const {
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

    fenStr.append(squareToStr(this->enPassSquare));
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
    if (this->enPassSquare != NULLSQUARE){
        this->zobristKey ^= Zobrist::enPassKeys[getFile(this->enPassSquare)];
    }
    // color to move
    if (!this->isWhiteTurn) {
        this->zobristKey ^= Zobrist::isBlackKey;
    }
    this->zobristKeyHistory = {this->zobristKey}; // synchronize history and current key
}

// makeMove will not check if the move is invalid
void Board::makeMove(BoardMove move) {
    const Square pos1 = move.sqr1();
    const Square pos2 = move.sqr2();
    const pieceTypes promotionPiece = move.getPromotePiece();

    // allies haven't made a move yet
    const pieceTypes allyKing = this->isWhiteTurn ? WKing : BKing;
    const pieceTypes allyRook = this->isWhiteTurn ? WRook : BRook;
    const pieceTypes allyPawn = this->isWhiteTurn ? WPawn : BPawn;
    const pieceTypes enemyRook = this->isWhiteTurn ? BRook : WRook;
    const int behindDirection = this->isWhiteTurn ? 8 : -8;

    const pieceTypes originPiece = this->getPiece(pos1);
    const pieceTypes targetPiece = this->getPiece(pos2);

    this->moveHistory.push_back(BoardState(
        BoardMove(pos1, pos2, promotionPiece),
        originPiece,
        targetPiece,
        this->castlingRights,
        this->enPassSquare,
        this->fiftyMoveRule
    ));

    const Square oldEnPassSquare = this->enPassSquare;
    const castleRights oldCastlingRights = this->castlingRights;

    this->setPiece(pos1, EmptyPiece); // origin square should be cleared in all situations
    this->setPiece(pos2, originPiece); // pretty much all possible moves translates the original piece to pos2

    // castling
    // doesn't check for emptiness between rook and king
    if (originPiece == allyKing && (this->castlingRights & castleRightsBit(pos2, this->isWhiteTurn))) {
        const int kingFileDirection = pos2 > pos1 ? 1 : -1;
        const int rookFile = kingFileDirection == 1 ? 7 : 0;
        this->setPiece(pos1 + kingFileDirection, allyRook);
        this->setPiece(pos1 - getFile(pos1) + rookFile, EmptyPiece);
        this->castlingRights &= this->isWhiteTurn ? B_Castle : W_Castle;
    }
    else if (originPiece == allyKing) {
        this->castlingRights &= this->isWhiteTurn ? B_Castle : W_Castle;
    }
    // jumping pawn
    else if (originPiece == allyPawn && abs(pos2 - pos1) == 16) {
        // doesn't check if pawn's original position is rank 2
        this->enPassSquare = pos2 + behindDirection;
        this->zobristKey ^= Zobrist::enPassKeys[getFile(pos2)];
    }
    // promoting pawn
    else if (promotionPiece != EmptyPiece) {
        this->setPiece(pos2, promotionPiece);
    }
    // en passant 
    else if (originPiece == allyPawn && pos2 == this->enPassSquare) {
        this->setPiece(pos2 + behindDirection, EmptyPiece);
        this->enPassSquare = NULLSQUARE;
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
        this->castlingRights &= pos1 == 63 ? NOT_W_OO  : All_Castle;
        this->castlingRights &= pos1 == 56 ? NOT_W_OOO : All_Castle;
        this->castlingRights &= pos1 == 7  ? NOT_B_OO  : All_Castle;
        this->castlingRights &= pos1 == 0  ? NOT_B_OOO : All_Castle;
    }
    if (targetPiece == enemyRook) {
        this->castlingRights &= pos2 == 63 ? NOT_W_OO  : All_Castle;
        this->castlingRights &= pos2 == 56 ? NOT_W_OOO : All_Castle;
        this->castlingRights &= pos2 == 7  ? NOT_B_OO  : All_Castle;
        this->castlingRights &= pos2 == 0  ? NOT_B_OOO : All_Castle;
    }

    // update zobrist key for changed castling rights; castling rights can only decrease in chess
    int mask;
    for (int i = 0; i < 4; i++) {
        mask = c_u64(1) << i;
        if ((oldCastlingRights & mask) && !(this->castlingRights & mask)) {
            this->zobristKey ^= Zobrist::castlingKeys[i];
        }
    }

    // handle old en passant square 
    if (this->enPassSquare == oldEnPassSquare) {
        this->enPassSquare = NULLSQUARE;
    }
    if (oldEnPassSquare != NULLSQUARE) {
        this->zobristKey ^= Zobrist::enPassKeys[getFile(oldEnPassSquare)];
    }	
    
    // after finalizing move logic, now switch turns
    this->isWhiteTurn = !this->isWhiteTurn; 
    this->zobristKey ^= Zobrist::isBlackKey;
    this->age++;

    // update history to include curr key
    this->zobristKeyHistory.push_back(this->zobristKey);
}

void Board::undoMove() {
    assert(this->moveHistory.size() != 0);

    const BoardState prev = moveHistory.back();

    const pieceTypes prevKing = this->isWhiteTurn ? BKing : WKing;
    const pieceTypes prevRook = this->isWhiteTurn ? BRook : WRook;
    const pieceTypes prevPawn = this->isWhiteTurn ? BPawn : WPawn;

    this->setPiece(prev.move.sqr1(), prev.originPiece);
    this->setPiece(prev.move.sqr2(), prev.targetPiece);

    // castling
    if (prev.originPiece == prevKing && (prev.castlingRights & castleRightsBit(prev.move.sqr2(), !this->isWhiteTurn)) ) {
        const int kingFileDirection = prev.move.sqr2() > prev.move.sqr1() ? 1 : -1;
        const int rookFile = kingFileDirection == 1 ? 7 : 0;
        this->setPiece(prev.move.sqr1() + kingFileDirection, EmptyPiece);
        this->setPiece(prev.move.sqr1() - getFile(prev.move.sqr1()) + rookFile, prevRook);
    }
    // en passant
    else if (prev.originPiece == prevPawn && prev.move.sqr2() == prev.enPassSquare) {
        const int behindDirection = this->isWhiteTurn ? -8 : 8;
        const pieceTypes prevJumpedPawn = this->isWhiteTurn ? WPawn : BPawn;
        this->setPiece(prev.move.sqr2() + behindDirection, prevJumpedPawn);
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

void Board::makeNullMove() {
    this->moveHistory.push_back(BoardState(
        BoardMove(),
        EmptyPiece,
        EmptyPiece,
        this->castlingRights,
        this->enPassSquare,
        this->fiftyMoveRule
    ));
    if (this->enPassSquare) {
        this->zobristKey ^= Zobrist::enPassKeys[getFile(this->enPassSquare)];
        this->enPassSquare = NULLSQUARE;
    }
    this->isWhiteTurn = !this->isWhiteTurn;
    this->fiftyMoveRule++;
    this->age++;

    this->zobristKey ^= Zobrist::isBlackKey;
    this->zobristKeyHistory.push_back(this->zobristKey);
}

void Board::unmakeNullMove() {
    this->enPassSquare = moveHistory.back().enPassSquare;
    this->isWhiteTurn = !this->isWhiteTurn;
    this->fiftyMoveRule--;
    this->age--;

    this->moveHistory.pop_back();
    this->zobristKeyHistory.pop_back();
    this->zobristKey = zobristKeyHistory.back();
}

bool Board::moveIsCapture(BoardMove move) const {
    if ( (this->getPiece(move.sqr1()) == WPawn || this->getPiece(move.sqr1()) == BPawn) && this->enPassSquare == move.sqr2())
        return true;
    if (move.getPromotePiece() == WQueen || move.getPromotePiece() == BQueen)
        return true;
    return this->getPiece(move.sqr2()) != EmptyPiece;
}

// getPiece is not responsible for bounds checking
pieceTypes Board::getPiece(Square square) const{
    return this->board[square];
}

// handles board, pieceSets, and zobristKey (not including en passant and castling)
void Board::setPiece(Square square, pieceTypes currPiece) {
    const uint64_t setSquare = (c_u64(1) << square);
    const uint64_t clearSquare = ALL_SQUARES ^ setSquare;

    const pieceTypes originPiece = this->getPiece(square);
    this->board[square] = currPiece;
    
    if (originPiece != EmptyPiece) {
        const pieceTypes originColor = originPiece < BKing ? WHITE_PIECES : BLACK_PIECES;

        this->pieceSets[originColor] &= clearSquare;
        this->pieceSets[originPiece] &= clearSquare;
        this->zobristKey ^= Zobrist::pieceKeys[originPiece][square];
        this->eval.removePiece(square, originPiece);
    }
    if (currPiece != EmptyPiece) {
        const pieceTypes currColor = currPiece < BKing ? WHITE_PIECES : BLACK_PIECES;

        this->pieceSets[currColor] ^= setSquare;
        this->pieceSets[currPiece] ^= setSquare;
        this->zobristKey ^= Zobrist::pieceKeys[currPiece][square];
        this->eval.addPiece(square, currPiece);
    }
}

bool Board::isLegalMove(const BoardMove move) const {
    // This is a bitboard implementation to check whether a move leaves the ally king under attack
    // The current move generation already checks whether castling is even valid 
    // or squares unblocked so only the king final position needs to be checked

    if (!move) {
        return false;
    }

    PieceSets tmpPieceSets = this->pieceSets;

    const uint64_t originSquare = (c_u64(1) << move.sqr1());
    const pieceTypes originColor = this->isWhiteTurn ? WHITE_PIECES : BLACK_PIECES;
    const pieceTypes originPiece = this->getPiece(move.sqr1());
    const pieceTypes allyPawn = this->isWhiteTurn ? WPawn : BPawn;
    const pieceTypes enemyPawn = this->isWhiteTurn ? BPawn : WPawn;

    // account for captures
    const uint64_t targetSquare = c_u64(1) << move.sqr2();
    const pieceTypes targetColor = this->isWhiteTurn ? BLACK_PIECES : WHITE_PIECES;
    pieceTypes targetPiece;
    
    // en passant
    if (originPiece == allyPawn && move.sqr2() == this->enPassSquare) {
        targetPiece = EmptyPiece;
        int dir = this->isWhiteTurn ? 8 : -8;
        uint64_t enemyPawnSquare = c_u64(1) << (move.sqr2() + dir);
        tmpPieceSets[targetColor] ^= enemyPawnSquare;
        tmpPieceSets[enemyPawn] ^= enemyPawnSquare;
    } else {
        targetPiece = this->getPiece(move.sqr2());
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
int Board::evaluate() {
    const int rawEval = this->eval.getRawEval(this->pieceSets);
    return this->isWhiteTurn ? rawEval : rawEval * -1;
}

bool operator==(const Board& lhs, const Board& rhs) {
    return lhs.board == rhs.board
        && lhs.pieceSets == rhs.pieceSets
        && lhs.zobristKeyHistory == rhs.zobristKeyHistory
        && lhs.isWhiteTurn == rhs.isWhiteTurn
        && lhs.enPassSquare == rhs.enPassSquare;
}

std::ostream& operator<<(std::ostream& os, const Board& target) {
    for (int rank = 0; rank <= 7; rank++) {
        os << "[";
        for (int file = 0; file <= 7; file++) {
            os << pieceToChar.at(target.getPiece(toSquare(rank, file)));
            os << ',';
        }
        os << "],\n";
    }
    os << "\n";
    os << "castlingRights: " << target.castlingRights << "\n";
    os << "enPassSquare: " << squareToStr(target.enPassSquare) << "\n";
    os << "isWhiteTurn: " << target.isWhiteTurn << "\n";
    os << "50MoveRule: " << target.fiftyMoveRule << "\n";
    os << "ZobristKey: " << target.zobristKey << "\n";
    return os;
}

castleRights castleRightsBit(Square finalKingPos, bool isWhiteTurn) {
    if (finalKingPos == 62 && isWhiteTurn) {
        return W_OO;
    }
    else if (finalKingPos == 58 && isWhiteTurn) {
        return W_OOO;
    }
    else if (finalKingPos == 6 && !isWhiteTurn) {
        return B_OO;
    }
    else if (finalKingPos == 2 && !isWhiteTurn) {
        return B_OOO;        
    }
    else {
        return noCastle;
    }
}

bool currKingInAttack(const PieceSets& pieceSets, bool isWhiteTurn) {
    const pieceTypes allyKing = isWhiteTurn ? WKing : BKing;
    assert(pieceSets[allyKing]);
    const int kingSquare = lsb(pieceSets[allyKing]);

    const uint64_t allPieces = pieceSets[WHITE_PIECES] | pieceSets[BLACK_PIECES];

    const uint64_t enemyKings   = isWhiteTurn ? pieceSets[BKing]   : pieceSets[WKing];
    const uint64_t enemyQueens  = isWhiteTurn ? pieceSets[BQueen]  : pieceSets[WQueen];
    const uint64_t enemyBishops = isWhiteTurn ? pieceSets[BBishop] : pieceSets[WBishop];
    const uint64_t enemyRooks   = isWhiteTurn ? pieceSets[BRook]   : pieceSets[WRook];
    const uint64_t enemyKnights = isWhiteTurn ? pieceSets[BKnight] : pieceSets[WKnight];
    const uint64_t enemyPawns   = isWhiteTurn ? pieceSets[BPawn]   : pieceSets[WPawn];

    return Attacks::bishopAttacks(kingSquare, allPieces) & (enemyBishops | enemyQueens)
        || Attacks::rookAttacks(kingSquare, allPieces) & (enemyRooks | enemyQueens)
        || Attacks::pawnAttacks(kingSquare, isWhiteTurn) & enemyPawns
        || Attacks::knightAttacks(kingSquare) & enemyKnights
        || Attacks::kingAttacks(kingSquare) & enemyKings;
}

uint64_t getAllPieces(const PieceSets& pieceSets) {
    return pieceSets[WHITE_PIECES] | pieceSets[BLACK_PIECES];
}