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
#include "types.hpp"
#include "eval.hpp" 

// Used for debugging and testing
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
    this->castlingRights = All_Castle;
    this->materialDifference = 0;
    this->eval = EvalAttributes();
    
    //sets up initial score for piece placement
    for(uint8_t i = 0; i < BOARD_SIZE; i++) {
        pieceTypes currPiece = board[i];
        this->eval.placementScore += Eval::getPlacementScore(i / 8, i % 8, currPiece, this->eval.gameState);
    }

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

    this->initZobristKey();
}

// Used for debugging and testing
Board::Board(std::array<pieceTypes, BOARD_SIZE> a_board, bool a_isWhiteTurn, 
            int a_fiftyMoveRule, BoardSquare a_pawnJumpedSquare, 
            castleRights a_castlingRights, int a_materialDifference) {
    this->board = a_board;
    this->isWhiteTurn = a_isWhiteTurn;
    this->fiftyMoveRule = a_fiftyMoveRule;
    this->pawnJumpedSquare = a_pawnJumpedSquare;
    this->castlingRights = a_castlingRights;
    this->materialDifference = a_materialDifference;

    uint8_t pieceCount = 0;
    uint8_t totalMaterial = 0;

    for(pieceTypes piece: board) {
        if(piece != EmptyPiece) {
            pieceCount++;
            totalMaterial += abs(pieceValues[piece]);
        }
    }
    this->eval = EvalAttributes(pieceCount, totalMaterial);
    this->eval.gameState = this->eval.piecesRemaining <= ENDGAME_PIECE_THRESHOLD ? Endgame : Opening;
    //sets up initial score for piece placement (This gets repeated so maybe could turn it into a function)
    for(uint8_t i = 0; i < BOARD_SIZE; i++) {
        pieceTypes currPiece = board[i];
        this->eval.placementScore += Eval::getPlacementScore(i / 8, i % 8, currPiece, eval.gameState);
    }

    for (int i = WKing; i < WHITE_PIECES; i++) {
        this->pieceSets[i] = makeBitboardFromArray(this->board, i);
        if (i < BKing) {
            this->pieceSets[WHITE_PIECES] |= makeBitboardFromArray(this->board, i);
        }
        else {
            this->pieceSets[BLACK_PIECES] |= makeBitboardFromArray(this->board, i);
        }
    }
    this->initZobristKey();
}

// Used in UCI
Board::Board(std::string fenStr) {
    this->materialDifference = 0;
    std::string token; 
    std::istringstream fenStream(fenStr);
    this->zobristKeyHistory = {0ull}; // required for setPiece
    this->eval = EvalAttributes(0, 0);

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

            this->materialDifference += pieceValues[currPiece];
            this->eval.piecesRemaining++;
            this->eval.totalMaterial += abs(pieceValues[currPiece]);

            file += 1;
        }

    }

    if(this->eval.piecesRemaining <= ENDGAME_PIECE_THRESHOLD)
        this->eval.gameState = Endgame;
    for(uint8_t i = 0; i < BOARD_SIZE; i++) {
        pieceTypes currPiece = board[i];
        this->eval.placementScore += Eval::getPlacementScore(i / 8, i % 8, currPiece, eval.gameState);
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

    fenStr.append(this->pawnJumpedSquare.toStr());
    fenStr.push_back(' ');

    fenStr.append(std::to_string(this->fiftyMoveRule));
    fenStr.push_back(' ');

    fenStr.push_back('1'); // Board doesn't use Fullmove counter

    return fenStr;
}

// assumes no move history
void Board::initZobristKey() {
    this->zobristKey = 0ull;

    // pieces on board
    for (size_t i = 0; i < BOARD_SIZE; i++) {
        pieceTypes currPiece = this->board[i];
        if (currPiece == EmptyPiece) {continue;}
        this->zobristKey ^= Zobrist::pieceKeys[currPiece][i];
    }
    // castling
    for (size_t i = 0; i < 4; i++) {
        if (this->castlingRights & 1ull << i) {
            this->zobristKey ^= Zobrist::castlingKeys[i];
        }
    }
    // en passant
    if (this->pawnJumpedSquare != BoardSquare()) {
        this->zobristKey ^= Zobrist::enPassKeys[this->pawnJumpedSquare.file];
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
        this->pawnJumpedSquare,
        this->fiftyMoveRule,
        this->materialDifference,
        this->eval
    ));

    BoardSquare oldPawnJumpedSquare = this->pawnJumpedSquare;
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
        this->pawnJumpedSquare = BoardSquare(pos2.rank + behindDirection, pos2.file);
        this->zobristKey ^= Zobrist::enPassKeys[pos2.file];
    }
    // promoting pawn
    else if (originPiece == allyPawn && pos2.rank == promotionRank) {
        this->setPiece(pos2, promotionPiece);

        //updates material score of the board on promotion
        if(this->isWhiteTurn) {
            this->materialDifference += pieceValues[promotionPiece] - 1;
        }
        else {
            this->materialDifference += pieceValues[promotionPiece] + 1;
        }
        this->eval.totalMaterial += abs(pieceValues[promotionPiece]) - 1;
    }
    // en passant 
    else if (originPiece == allyPawn && pos2 == this->pawnJumpedSquare) {
        this->setPiece(pos1.rank, pos2.file, EmptyPiece);
        this->pawnJumpedSquare = BoardSquare();
        this->eval.piecesRemaining--;
        this->eval.totalMaterial--;

        if(this->eval.piecesRemaining <= ENDGAME_PIECE_THRESHOLD)
            this->eval.gameState = Endgame;

        if(this->isWhiteTurn)
            materialDifference++;
        else
            materialDifference--;
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
        int mask = 1ull << i;
        if ((oldCastlingRights & mask) && !(this->castlingRights & mask)) {
            this->zobristKey ^= Zobrist::castlingKeys[i];
        }
    }

    // updates the material score and piece count of the board on capture
    if (targetPiece != EmptyPiece) {
        this->materialDifference -= pieceValues[targetPiece];
        this->eval.piecesRemaining--; 
        this->eval.totalMaterial -= abs(pieceValues[targetPiece]);
        if(this->eval.gameState != Endgame && this->eval.piecesRemaining <= ENDGAME_PIECE_THRESHOLD) {
            this->eval.gameState = Endgame;
        }
            
    }

    //This is inefficient and hopefully temporary
    if(this->eval.piecesRemaining <= ENDGAME_PIECE_THRESHOLD) {
        for(uint8_t i = 0; i < BOARD_SIZE; i++) {
        pieceTypes currPiece = board[i];
        this->eval.placementScore += Eval::getPlacementScore(i / 8, i % 8, currPiece, eval.gameState);
        }
    }

    
    // handle old en passant square 
    if (this->pawnJumpedSquare == oldPawnJumpedSquare) {
        this->pawnJumpedSquare = BoardSquare(); 
    }
    if (oldPawnJumpedSquare != BoardSquare()) {
        this->zobristKey ^= Zobrist::enPassKeys[oldPawnJumpedSquare.file];
    }	
    
    // after finalizing move logic, now switch turns
    this->isWhiteTurn = !this->isWhiteTurn; 
    this->zobristKey ^= Zobrist::isBlackKey;

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
    else if (prev.originPiece == prevPawn && prev.move.pos2 == prev.pawnJumpedSquare) {
        pieceTypes prevJumpedPawn = prevPawn == BPawn ? WPawn : BPawn;
        this->setPiece(prev.move.pos1.rank, prev.move.pos2.file, prevJumpedPawn);
    }

    this->isWhiteTurn = !this->isWhiteTurn;
    this->castlingRights = prev.castlingRights;
    this->pawnJumpedSquare = prev.pawnJumpedSquare;
    this->fiftyMoveRule = prev.fiftyMoveRule;
    this->materialDifference = prev.materialDifference;
    this->eval = prev.eval;

    this->moveHistory.pop_back();
    this->zobristKeyHistory.pop_back();
    this->zobristKey = zobristKeyHistory.back();
}

bool Board::isLegalMove(const BoardMove move) const {
    // This is a bitboard implementation to check whether a move leaves the ally king under attack
    // The current move generation already checks whether castling is even valid 
    // or squares unblocked so only the king final position needs to be checked

    assert(move.isValid());

    std::array<uint64_t, NUM_BITBOARDS> tmpPieceSets = this->pieceSets;
    pieceTypes originColor = this->isWhiteTurn ? WHITE_PIECES : BLACK_PIECES;

    uint64_t originSquare = (1ull << move.pos1.toSquare());
    uint64_t targetSquare = (1ull << move.pos2.toSquare());
    pieceTypes originPiece = this->getPiece(move.pos1);
    pieceTypes targetPiece = this->getPiece(move.pos2);

    // move ally piece 
    tmpPieceSets[originColor] ^= originSquare;
    tmpPieceSets[originPiece] ^= originSquare;
    tmpPieceSets[originColor] ^= targetSquare;
    tmpPieceSets[originPiece] ^= targetSquare;

    if (targetPiece != EmptyPiece) {
        pieceTypes targetColor = this->isWhiteTurn ? BLACK_PIECES : WHITE_PIECES;
        tmpPieceSets[targetColor] ^= targetSquare;
        tmpPieceSets[targetPiece] ^= targetSquare;
    }
    
    return !currKingInAttack(tmpPieceSets, this->isWhiteTurn);
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
    uint64_t setSquare = (1ull << square);
    uint64_t clearSquare = ALL_SQUARES ^ setSquare;

    pieceTypes originPiece = this->getPiece(rank, file);
    this->board[square] = currPiece;
    
    if (originPiece != EmptyPiece) {
        pieceTypes originColor = originPiece < BKing ? WHITE_PIECES : BLACK_PIECES;
        this->pieceSets[originColor] &= clearSquare;
        this->pieceSets[originPiece] &= clearSquare;
        this->zobristKey ^= Zobrist::pieceKeys[originPiece][square];

        this->eval.placementScore -= Eval::getPlacementScore(rank, file, originPiece, this->eval.gameState);
    }
    if (currPiece != EmptyPiece) {
        pieceTypes currColor = currPiece < BKing ? WHITE_PIECES : BLACK_PIECES;
        this->pieceSets[currColor] ^= setSquare;
        this->pieceSets[currPiece] ^= setSquare;
        this->zobristKey ^= Zobrist::pieceKeys[currPiece][square];

        this->eval.placementScore += Eval::getPlacementScore(rank, file, currPiece, this->eval.gameState);
    }
}
    
int Board::getEvalScore() const {
    int scoreSum = this->eval.placementScore;
    return this->isWhiteTurn ? scoreSum : scoreSum * -1;
}

void Board::setPiece(BoardSquare square, pieceTypes currPiece) {
    this->setPiece(square.rank, square.file, currPiece);
}

bool operator==(const Board& lhs, const Board& rhs) {
    return  (lhs.board == rhs.board) && (lhs.pieceSets == rhs.pieceSets) && (lhs.zobristKeyHistory == rhs.zobristKeyHistory);
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

bool currKingInAttack(std::array<uint64_t, NUM_BITBOARDS>& pieceSets, bool isWhiteTurn) {
    pieceTypes allyKing = isWhiteTurn ? WKing : BKing;
    int kingSquare = leadingBit(pieceSets[allyKing]);
    assert(kingSquare != -1);

    uint64_t allPieces = pieceSets[WHITE_PIECES] | pieceSets[BLACK_PIECES];

    uint64_t enemyKings   = isWhiteTurn ? pieceSets[BKing]   : pieceSets[WKing];
    uint64_t enemyQueens  = isWhiteTurn ? pieceSets[BQueen]  : pieceSets[WQueen];
    uint64_t enemyBishops = isWhiteTurn ? pieceSets[BBishop] : pieceSets[WBishop];
    uint64_t enemyRooks   = isWhiteTurn ? pieceSets[BRook]   : pieceSets[WRook];
    uint64_t enemyKnights = isWhiteTurn ? pieceSets[BKnight] : pieceSets[WKnight];
    uint64_t enemyPawns   = isWhiteTurn ? pieceSets[BPawn]   : pieceSets[WPawn];

    return Attacks::bishopAttacks(kingSquare, allPieces) & (enemyBishops | enemyQueens)
        || Attacks::rookAttacks(kingSquare, allPieces) & (enemyRooks | enemyQueens)
        || knightSquares(enemyKnights) & 1ull << kingSquare
        || pawnAttackers(kingSquare, enemyPawns, isWhiteTurn)
        || kingAttackers(kingSquare, enemyKings);
}

// used for debugging
uint64_t makeBitboardFromArray(std::array<pieceTypes, BOARD_SIZE> board, int target) {
    uint64_t result = 0ull;
    for (size_t i = 0; i < board.size(); i++) {
        if (board.at(i) == target) {
            result |= (1ull << i);
        }
    }
    return result;
}

bool Board::moveIsCapture(BoardMove move) {
    if(this->getPiece(move.pos1) % 6 == WPawn && this->pawnJumpedSquare == move.pos2)
        return true;

    return this->getPiece(move.pos2) != EmptyPiece;
}
