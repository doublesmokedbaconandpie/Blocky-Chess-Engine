#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include "extract.hpp"
#include "moveGen.hpp"
#include "board.hpp"
#include "move.hpp"
#include "attacks.hpp"
#include "types.hpp"

// This program is meant to convert the pgns from a Cutechess match into a data format
// that is easy to parse; make sure not to have any incompleted games within those pgns

int main() {
    Attacks::init();

    std::string extractDest = "../data.pgn";
    std::vector<std::string> cutechessPgns = {"../endgames.pgn", "../endgames2.pgn"};

    std::ofstream dest(extractDest);
    for (std::string cutechessPgn: cutechessPgns) {
        std::ifstream file(cutechessPgn);
        while (!file.eof()) {
            WinningColor result = getGameResult(file);
            std::vector<std::string> fens = getFens(file, result);
            storeFenResults(dest, fens, result);
        }
    }
    return 0;
}

WinningColor getGameResult(std::ifstream& file) {
    // Results are stored in the following form when using Cutechess: 
    // [Result "0-1"] or [Result "1/2-1/2"]
    std::string token;
    while (file >> token) {
        if (token == "[Result") {
            file >> token;
            break;
        }
    }

    // check file open still
    if (file.eof()) {
        return WHITE; // return value doesn't matter
    }

    std::string result = token.substr(1, token.length() - 3);
    if (result == "1-0") {return WHITE;}
    if (result == "1/2-1/2") {return DRAW;}
    if (result == "0-1") {return BLACK;}
    throw std::runtime_error("Invalid result: " + token);
}

std::vector<std::string> getFens(std::ifstream& file, const WinningColor result) {
    // Games are generally stored in the following format:
    // 1. Nf3 {book} Nc6 {book} 2. b3 {book} d6 {book}
    // ...
    // 30. Ke2 {-M2/5 0.20s} Ng3# {+M1/5 0.20s, Black mates} 0-1

    std::vector<std::string> fens;
    std::string token;

    // check for file open
    if (file.eof()) {
        return fens;
    }

    // get to moves
    while (token != "1."){
        file >> token;
    }
    
    // read moves
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BoardMove move;
    while (token != toStr(result)) {
        // two sides make moves
        for (int i = 0; i < 2; i++) {
            // move
            file >> token;
            if (token == toStr(result)) {
                return fens;
            }
            move = getMove(token, board);

            // move annotation
            while (token.back() != '}') {
                file >> token;
            }
            if (token != "{book}") {
                fens.push_back(board.toFen());
            }
        }
        file >> token; // load the next move number
    }
    return fens;
}

BoardMove getMove(std::string input, Board& board) {
    auto moves = MoveGen::moveGenerator(board);
    std::string origInput = input;

    // checkmate or check (Ex: Ng3+), trim the last character
    if (input.back() == '#' || input.back() == '+') {
        input = input.substr(0, input.length() - 1);
    }

    // castles (Ex: O-O)
    if (input == "O-O" || input == "O-O-O") {
        int castleFile = input == "O-O" ? 6 : 2;
        pieceTypes allyKing = board.isWhiteTurn ? WKing : BKing;
        for (BoardMove move: moves) {
            if (board.getPiece(move.pos1) == allyKing 
                && castleRightsBit(move.pos2, board.isWhiteTurn)
                && move.pos2.file == castleFile) {
                board.makeMove(move);
                return move;
            }
        }
        throw std::runtime_error("Did not detect castle: " + input + " with fen " + board.toFen());
    }

    // pawn promotions (Ex: e8=Q), trim the last two characters
    pieceTypes promotePiece = EmptyPiece;
    if (input[input.length() - 2] == '=') {
        char pieceChar = board.isWhiteTurn ? input.back() : tolower(input.back());
        promotePiece = charToPiece.at(pieceChar);
        input = input.substr(0, input.length() - 2);
    }

    // piece captures (Ex: exd5), trim the character
    if (input.find('x') != std::string::npos) {
        input.erase(std::remove(input.begin(), input.end(), 'x'), input.end());
    }

    // case where there is a piece prefix (Ex: Nf3); trim first character
    pieceTypes currPiece = EmptyPiece;
    if (isupper(input[0])) {
        char pieceChar = board.isWhiteTurn ? input[0] : tolower(input[0]);
        currPiece = charToPiece.at(pieceChar);
        input.erase(input.begin());
    }

    // case where there is a file qualifier, trim first character
    int file = -1;
    if (isalpha(input[0]) && input.length() > 2) {
        file = input[0] - 'a';
        input.erase(input.begin());
    }

    // case where there is a rank qualifier, trim first character
    int rank = -1;
    if (isdigit(input[0]) && input.length() > 2) {
        rank = 7 - (input[0] - '1');
        input.erase(input.begin());
    }

    // last two characters are guaranteed to be a destination square
    BoardSquare dest(input);

    // filter moves
    auto notCurrPiece = [board, currPiece](BoardMove move) {
        return board.getPiece(move.pos1) != currPiece;};
    if (currPiece != EmptyPiece) {
        moves.erase(std::remove_if(moves.begin(), moves.end(), notCurrPiece), moves.end());
    }

    auto notPromotePiece = [board, promotePiece](BoardMove move) {
        return move.promotionPiece != promotePiece;};
    if (promotePiece != EmptyPiece) {
        moves.erase(std::remove_if(moves.begin(), moves.end(), notPromotePiece), moves.end());
    }
    
    auto notRank = [board, rank](BoardMove move) {
        return move.pos1.rank != rank;};
    if (rank != -1) {
        moves.erase(std::remove_if(moves.begin(), moves.end(), notRank), moves.end());
    }
    
    auto notFile = [board, file](BoardMove move) {
        return move.pos1.file != file;};
    if (file != -1) {
        moves.erase(std::remove_if(moves.begin(), moves.end(), notFile), moves.end());
    }

    auto notDest = [board, dest](BoardMove move) {
        return move.pos2 != dest;
    };
    moves.erase(std::remove_if(moves.begin(), moves.end(), notDest), moves.end());

    // default pawn makes move
    if (moves.size() > 1) {
        pieceTypes allyPawn = board.isWhiteTurn ? WPawn : BPawn;
        auto notPawn = [board, allyPawn](BoardMove move) {return board.getPiece(move.pos1) != allyPawn;};
        moves.erase(std::remove_if(moves.begin(), moves.end(), notPawn), moves.end());
    }

    if (moves.size() != 1) {
        std::cout << moves.size() << std::endl;
        throw std::runtime_error("Move not selected for: " + origInput + " with fen " + board.toFen());
    }
    board.makeMove(moves[0]);
    return moves[0];
}

void storeFenResults(std::ofstream& file, std::vector<std::string> fens, WinningColor result) {
    std::string resultStr = toStr(result);
    for (std::string fen: fens) {
        file << fen << " ; " << resultStr << '\n';
    }
}

std::string toStr(const WinningColor result) {
    if (result == WHITE) {return "1-0";}
    else if (result == DRAW) {return "1/2-1/2";}
    else if (result == BLACK) {return "0-1";}
    return "";
}