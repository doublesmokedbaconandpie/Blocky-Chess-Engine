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

    this->m_zobristKey = 0;
    this->m_zobristKeyHistory = {0}; // required for setPiece

    std::fill(this->m_board.begin(), this->m_board.end(), EmptyPiece);
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
    this->m_isWhiteTurn = token == "w" ? true : false;

    fenStream >> token;
    this->m_castlingRights = noCastle;
    this->m_castlingRights ^= token.find('K') != std::string::npos ? W_OO : noCastle;
    this->m_castlingRights ^= token.find('Q') != std::string::npos ? W_OOO : noCastle;
    this->m_castlingRights ^= token.find('k') != std::string::npos ? B_OO : noCastle;
    this->m_castlingRights ^= token.find('q') != std::string::npos ? B_OOO : noCastle;

    fenStream >> token;
    this->m_enPassSquare = toSquare(token);

    fenStream >> token;
    this->m_fiftyMoveRule = stoi(token);

    this->initZobristKey();
    // Board doesn't use Fullmove counter
}

auto Board::toFen() const -> std::string{
    std::string fenStr; 

    int emptyPiecesInRow = 0, piecesPlaced = 0;
    for (pieceTypes piece: this->m_board) {
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

    this->m_isWhiteTurn ? fenStr.append("w") : fenStr.append("b");
    fenStr.push_back(' ');

    this->m_castlingRights & W_OO  ? fenStr.append("K") : "";
    this->m_castlingRights & W_OOO ? fenStr.append("Q") : "";
    this->m_castlingRights & B_OO  ? fenStr.append("k") : "";
    this->m_castlingRights & B_OOO ? fenStr.append("q") : "";
    this->m_castlingRights == noCastle ? fenStr.append("-") : "";
    fenStr.push_back(' ');

    fenStr.append(squareToStr(this->m_enPassSquare));
    fenStr.push_back(' ');

    fenStr.append(std::to_string(this->m_fiftyMoveRule));
    fenStr.push_back(' ');

    fenStr.push_back('1'); // Board doesn't use Fullmove counter

    return fenStr;
}

// assumes no move history
void Board::initZobristKey() {
    // castling
    for (size_t i = 0; i < 4; i++) {
        if (this->m_castlingRights & c_u64(1) << i) {
            this->m_zobristKey ^= Zobrist::castlingKeys[i];
        }
    }
    // en passant
    if (this->m_enPassSquare != NULLSQUARE){
        this->m_zobristKey ^= Zobrist::enPassKeys[getFile(this->m_enPassSquare)];
    }
    // color to move
    if (!this->m_isWhiteTurn) {
        this->m_zobristKey ^= Zobrist::isBlackKey;
    }
    this->m_zobristKeyHistory = {this->m_zobristKey}; // synchronize history and current key
}

// makeMove will not check if the move is invalid
void Board::makeMove(Move move) {
    const Square pos1 = move.sqr1();
    const Square pos2 = move.sqr2();
    const pieceTypes promotionPiece = move.getPromotePiece();

    // allies haven't made a move yet
    const pieceTypes allyKing = this->m_isWhiteTurn ? WKing : BKing;
    const pieceTypes allyRook = this->m_isWhiteTurn ? WRook : BRook;
    const pieceTypes allyPawn = this->m_isWhiteTurn ? WPawn : BPawn;
    const pieceTypes enemyRook = this->m_isWhiteTurn ? BRook : WRook;
    const int behindDirection = this->m_isWhiteTurn ? 8 : -8;

    const pieceTypes originPiece = this->getPiece(pos1);
    const pieceTypes targetPiece = this->getPiece(pos2);

    this->m_moveHistory.push_back(BoardState(
        Move(pos1, pos2, promotionPiece),
        originPiece,
        targetPiece,
        this->m_castlingRights,
        this->m_enPassSquare,
        this->m_fiftyMoveRule
    ));

    const Square oldEnPassSquare = this->m_enPassSquare;
    const castleRights oldCastlingRights = this->m_castlingRights;

    this->setPiece(pos1, EmptyPiece); // origin square should be cleared in all situations
    this->setPiece(pos2, originPiece); // pretty much all possible moves translates the original piece to pos2

    // castling
    // doesn't check for emptiness between rook and king
    if (originPiece == allyKing && (this->m_castlingRights & castleRightsBit(pos2, this->m_isWhiteTurn))) {
        const int kingFileDirection = pos2 > pos1 ? 1 : -1;
        const int rookFile = kingFileDirection == 1 ? 7 : 0;
        this->setPiece(pos1 + kingFileDirection, allyRook);
        this->setPiece(pos1 - getFile(pos1) + rookFile, EmptyPiece);
        this->m_castlingRights &= this->m_isWhiteTurn ? B_Castle : W_Castle;
    }
    else if (originPiece == allyKing) {
        this->m_castlingRights &= this->m_isWhiteTurn ? B_Castle : W_Castle;
    }
    // jumping pawn
    else if (originPiece == allyPawn && abs(pos2 - pos1) == 16) {
        // doesn't check if pawn's original position is rank 2
        this->m_enPassSquare = pos2 + behindDirection;
        this->m_zobristKey ^= Zobrist::enPassKeys[getFile(pos2)];
    }
    // promoting pawn
    else if (promotionPiece != EmptyPiece) {
        this->setPiece(pos2, promotionPiece);
    }
    // en passant 
    else if (originPiece == allyPawn && pos2 == this->m_enPassSquare) {
        this->setPiece(pos2 + behindDirection, EmptyPiece);
        this->m_enPassSquare = NULLSQUARE;
    }

    // reset fifty move rule on captures or pawn moves
    if (targetPiece != EmptyPiece || originPiece == allyPawn) {
        this->m_fiftyMoveRule = 0;
    }
    else {
        this->m_fiftyMoveRule++;
    }

    // if either your allyRook is moved or an enemyRook is captured, modify castling rights
    if (originPiece == allyRook) {
        this->m_castlingRights &= pos1 == 63 ? NOT_W_OO  : All_Castle;
        this->m_castlingRights &= pos1 == 56 ? NOT_W_OOO : All_Castle;
        this->m_castlingRights &= pos1 == 7  ? NOT_B_OO  : All_Castle;
        this->m_castlingRights &= pos1 == 0  ? NOT_B_OOO : All_Castle;
    }
    if (targetPiece == enemyRook) {
        this->m_castlingRights &= pos2 == 63 ? NOT_W_OO  : All_Castle;
        this->m_castlingRights &= pos2 == 56 ? NOT_W_OOO : All_Castle;
        this->m_castlingRights &= pos2 == 7  ? NOT_B_OO  : All_Castle;
        this->m_castlingRights &= pos2 == 0  ? NOT_B_OOO : All_Castle;
    }

    // update zobrist key for changed castling rights; castling rights can only decrease in chess
    int mask;
    for (int i = 0; i < 4; i++) {
        mask = c_u64(1) << i;
        if ((oldCastlingRights & mask) && !(this->m_castlingRights & mask)) {
            this->m_zobristKey ^= Zobrist::castlingKeys[i];
        }
    }

    // handle old en passant square 
    if (this->m_enPassSquare == oldEnPassSquare) {
        this->m_enPassSquare = NULLSQUARE;
    }
    if (oldEnPassSquare != NULLSQUARE) {
        this->m_zobristKey ^= Zobrist::enPassKeys[getFile(oldEnPassSquare)];
    }	
    
    // after finalizing move logic, now switch turns
    this->m_isWhiteTurn = !this->m_isWhiteTurn;
    this->m_zobristKey ^= Zobrist::isBlackKey;
    this->m_age++;

    // update history to include curr key
    this->m_zobristKeyHistory.push_back(this->m_zobristKey);
}

void Board::undoMove() {
    assert(this->m_moveHistory.size() != 0);

    const BoardState prev = this->m_moveHistory.back();

    const pieceTypes prevKing = this->m_isWhiteTurn ? BKing : WKing;
    const pieceTypes prevRook = this->m_isWhiteTurn ? BRook : WRook;
    const pieceTypes prevPawn = this->m_isWhiteTurn ? BPawn : WPawn;

    this->setPiece(prev.move.sqr1(), prev.originPiece);
    this->setPiece(prev.move.sqr2(), prev.targetPiece);

    // castling
    if (prev.originPiece == prevKing && (prev.castlingRights & castleRightsBit(prev.move.sqr2(), !this->m_isWhiteTurn)) ) {
        const int kingFileDirection = prev.move.sqr2() > prev.move.sqr1() ? 1 : -1;
        const int rookFile = kingFileDirection == 1 ? 7 : 0;
        this->setPiece(prev.move.sqr1() + kingFileDirection, EmptyPiece);
        this->setPiece(prev.move.sqr1() - getFile(prev.move.sqr1()) + rookFile, prevRook);
    }
    // en passant
    else if (prev.originPiece == prevPawn && prev.move.sqr2() == prev.enPassSquare) {
        const int behindDirection = this->m_isWhiteTurn ? -8 : 8;
        const pieceTypes prevJumpedPawn = this->m_isWhiteTurn ? WPawn : BPawn;
        this->setPiece(prev.move.sqr2() + behindDirection, prevJumpedPawn);
    }

    this->m_isWhiteTurn = !this->m_isWhiteTurn;
    this->m_castlingRights = prev.castlingRights;
    this->m_enPassSquare = prev.enPassSquare;
    this->m_fiftyMoveRule = prev.fiftyMoveRule;
    this->m_age--;

    this->m_moveHistory.pop_back();
    this->m_zobristKeyHistory.pop_back();
    this->m_zobristKey = m_zobristKeyHistory.back();
}

void Board::makeNullMove() {
    this->m_moveHistory.push_back(BoardState(
        Move(),
        EmptyPiece,
        EmptyPiece,
        this->m_castlingRights,
        this->m_enPassSquare,
        this->m_fiftyMoveRule
    ));
    if (this->m_enPassSquare) {
        this->m_zobristKey ^= Zobrist::enPassKeys[getFile(this->m_enPassSquare)];
        this->m_enPassSquare = NULLSQUARE;
    }
    this->m_isWhiteTurn = !this->m_isWhiteTurn;
    this->m_fiftyMoveRule++;
    this->m_age++;

    this->m_zobristKey ^= Zobrist::isBlackKey;
    this->m_zobristKeyHistory.push_back(this->m_zobristKey);
}

void Board::unmakeNullMove() {
    this->m_enPassSquare = this->m_moveHistory.back().enPassSquare;
    this->m_isWhiteTurn = !this->m_isWhiteTurn;
    this->m_fiftyMoveRule--;
    this->m_age--;

    this->m_moveHistory.pop_back();
    this->m_zobristKeyHistory.pop_back();
    this->m_zobristKey = m_zobristKeyHistory.back();
}

// getPiece is not responsible for bounds checking
auto Board::getPiece(Square square) const -> pieceTypes{
    return this->m_board[square];
}

// handles board, pieceSets, and zobristKey (not including en passant and castling)
void Board::setPiece(Square square, pieceTypes currPiece) {
    const uint64_t setSquare = (c_u64(1) << square);
    const uint64_t clearSquare = ALL_SQUARES ^ setSquare;

    const pieceTypes originPiece = this->getPiece(square);
    this->m_board[square] = currPiece;
    
    if (originPiece != EmptyPiece) {
        const pieceTypes originColor = originPiece < BKing ? WHITE_PIECES : BLACK_PIECES;

        this->pieceSets[originColor] &= clearSquare;
        this->pieceSets[originPiece] &= clearSquare;
        this->m_zobristKey ^= Zobrist::pieceKeys[originPiece][square];
        this->eval.removePiece(square, originPiece);
    }
    if (currPiece != EmptyPiece) {
        const pieceTypes currColor = currPiece < BKing ? WHITE_PIECES : BLACK_PIECES;

        this->pieceSets[currColor] ^= setSquare;
        this->pieceSets[currPiece] ^= setSquare;
        this->m_zobristKey ^= Zobrist::pieceKeys[currPiece][square];
        this->eval.addPiece(square, currPiece);
    }
}

auto Board::isLegalMove(const Move move) const -> bool {
    // This is a bitboard implementation to check whether a move leaves the ally king under attack
    // The current move generation already checks whether castling is even valid 
    // or squares unblocked so only the king final position needs to be checked

    if (!move) {
        return false;
    }

    PieceSets tmpPieceSets = this->pieceSets;

    const uint64_t originSquare = (c_u64(1) << move.sqr1());
    const pieceTypes originColor = this->m_isWhiteTurn ? WHITE_PIECES : BLACK_PIECES;
    const pieceTypes originPiece = this->getPiece(move.sqr1());
    const pieceTypes allyPawn = this->m_isWhiteTurn ? WPawn : BPawn;
    const pieceTypes enemyPawn = this->m_isWhiteTurn ? BPawn : WPawn;

    // account for captures
    const uint64_t targetSquare = c_u64(1) << move.sqr2();
    const pieceTypes targetColor = this->m_isWhiteTurn ? BLACK_PIECES : WHITE_PIECES;
    pieceTypes targetPiece;
    
    // en passant
    if (originPiece == allyPawn && move.sqr2() == this->m_enPassSquare) {
        targetPiece = EmptyPiece;
        int dir = this->m_isWhiteTurn ? 8 : -8;
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

    return !currKingInAttack(tmpPieceSets, this->m_isWhiteTurn);
}

auto Board::moveIsCapture(Move move) const -> bool {
    if ( (this->getPiece(move.sqr1()) == WPawn || this->getPiece(move.sqr1()) == BPawn) && this->m_enPassSquare == move.sqr2())
        return true;
    if (move.getPromotePiece() == WQueen || move.getPromotePiece() == BQueen)
        return true;
    return this->getPiece(move.sqr2()) != EmptyPiece;
}

bool Board::isDraw() const {
    // fifty move rule
    if (this->m_fiftyMoveRule >= 100) {
        return true;
    }
    // 3fold repetition
    const int occurrences = std::count(this->m_zobristKeyHistory.begin(), this->m_zobristKeyHistory.end(), this->m_zobristKey);
    return occurrences >= 3 ? true : false;
}
    
// positive return values means winning for the side to move, negative is opposite
auto Board::evaluate() -> int {
    const int rawEval = this->eval.getRawEval(this->pieceSets);
    return this->m_isWhiteTurn ? rawEval : rawEval * -1;
}

auto Board::lastMoveCaptureOrCastle() const -> bool {
    return this->m_moveHistory.back().targetPiece != EmptyPiece
        || this->m_castlingRights != this->m_moveHistory.back().castlingRights;
}

void Board::clearHistory() {
    this->m_moveHistory.clear();
    this->m_zobristKeyHistory.clear();
    this->m_zobristKeyHistory.push_back(this->m_zobristKey);
}

// for tests
auto Board::operator==(const Board& rhs) const -> bool {
    return this->board() == rhs.board()
        && this->pieceSets == rhs.pieceSets
        && this->isWhiteTurn() == rhs.isWhiteTurn()
        && this->fiftyMoveRule() == rhs.fiftyMoveRule()
        && this->zobristKey() == rhs.zobristKey()
        && this->zobristKey() != 0
        && rhs.zobristKey() != 0
        && this->enPassSquare() == rhs.enPassSquare();
}

auto Board::operator<<(std::ostream& os) const -> std::ostream& {
    for (int rank = 0; rank <= 7; rank++) {
        os << "[";
        for (int file = 0; file <= 7; file++) {
            os << pieceToChar.at(this->getPiece(toSquare(rank, file)));
            os << ',';
        }
        os << "],\n";
    }
    os << "\n";
    os << "castlingRights: " << this->castlingRights() << "\n";
    os << "enPassSquare: " << squareToStr(this->enPassSquare()) << "\n";
    os << "isWhiteTurn: " << this->isWhiteTurn() << "\n";
    os << "50MoveRule: " << this->fiftyMoveRule() << "\n";
    os << "ZobristKey: " << this->zobristKey() << "\n";
    return os;
}

auto castleRightsBit(Square finalKingPos, bool m_isWhiteTurn) -> castleRights {
    if (finalKingPos == 62 && m_isWhiteTurn) {
        return W_OO;
    }
    else if (finalKingPos == 58 && m_isWhiteTurn) {
        return W_OOO;
    }
    else if (finalKingPos == 6 && !m_isWhiteTurn) {
        return B_OO;
    }
    else if (finalKingPos == 2 && !m_isWhiteTurn) {
        return B_OOO;        
    }
    else {
        return noCastle;
    }
}

auto currKingInAttack(const PieceSets& pieceSets, bool isWhiteTurn) -> bool {
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

auto getAllPieces(const PieceSets& pieceSets) -> uint64_t {
    return pieceSets[WHITE_PIECES] | pieceSets[BLACK_PIECES];
}