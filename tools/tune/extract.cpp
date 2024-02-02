#include <cassert>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include "extract.hpp"
#include "moveOrder.hpp"
#include "moveGen.hpp"
#include "board.hpp"
#include "move.hpp"
#include "attacks.hpp"
#include "bitboard.hpp"
#include "zobrist.hpp" // for rand64()
#include "types.hpp"

// This program is meant to convert the pgns from a Cutechess match into a data format
// that is easy to parse; make sure not to have any incompleted games within those pgns

std::array<uint64_t, 4> seed = {0xf38f4541449b0fc3, 0x8432cf48703f8864, 0x1c8596ae5c1621d1, 0xf6d3be81a796f876};

constexpr int MOD_FENS = 3;

int main() {
    // initialize prerequisites
    Attacks::init();
    std::vector<std::string> pgns, dests;
    readFileNames(pgns, dests);

    // print filenames
    std::cout << "Files to process:\n";
    assert(pgns.size() == dests.size());
    for (int i = 0; i < pgns.size(); ++i) {
        std::cout << "Pgn: " << pgns[i] << " Dest: " << dests[i] << '\n';
    }
    std::cout << '\n';

    // main file reading loop
    int games = 0;
    for (int i = 0; i < dests.size(); ++i) {
        const auto pgn = pgns[i];
        const auto destName = dests[i];
        std::cout << "Processing file: " << pgn << ", Dest: " << destName << std::endl;
        extractFromFile(pgn, destName, games);
    }
    std::cout << "Total games processed: " << games << std::endl;
    return 0;
}

void extractFromFile(std::string pgn, std::string destName, int& games) {
    std::ifstream file(pgn);
    std::ofstream dest(destName);
    assert(file);
    assert(dest);

    while (!file.eof()) {
        bool blackStarts;
        const auto result = getGameResult(file);
        const auto startFen = getStartFen(file, blackStarts);
        const auto fens = getPositions(file, startFen, result, blackStarts);
        storeFenResults(dest, fens, result);

        if (result != NA) {
            ++games;
        }
        if (games % 500 == 0) {
            std::cout << "Games processed: " << games << std::endl;
        }
    }
}

// reads pgn names from ../sources.csv 
// sources.csv has each line with a cutechess pgn input and output
// Ex: main.pgn,main.txt
void readFileNames(std::vector<std::string>& pgns, std::vector<std::string>& dests) {
    std::ifstream file("sources.csv");
    std::string pgn, dest;
    while (!file.eof()) {
        // read a line in the csv
        std::getline(file, pgn, ',');
        std::getline(file, dest);
        // remove newlines
        pgn.erase(std::remove(pgn.begin(), pgn.end(), '\n'), pgn.end());
        dest.erase(std::remove(dest.begin(), dest.end(), '\n'), dest.end());
        // save corresponding input and outputs
        if (std::filesystem::is_regular_file(pgn) && dest != "") {
            pgns.push_back(pgn);
            dests.push_back(dest);
        }
    }
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
        return NA; // return value doesn't matter
    }

    std::string result = token.substr(1, token.length() - 3);
    if (result == "1-0") {return WHITE;}
    if (result == "1/2-1/2") {return DRAW;}
    if (result == "0-1") {return BLACK;}
    // happens if cutechess is interrupted, result doesn't matter
    if (result == "*") {return NA;}
    throw std::runtime_error("Invalid result: " + token);
}

// for each game, there is a starting position, which can then be incremented with moves
// some opening books use moves from startpos, others start from a fen
std::string getStartFen(std::ifstream& file, bool& blackStarts) {
    std::string token, fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    while (token != "1." && token != "1..." && !file.eof()) {
        file >> token;
        // once fen is updated, keep churning tokens until the first move is found
        if (token == "[FEN") {
            std::getline(file, fen, ']');
            fen.erase(0, 2);
            fen.erase(fen.size() - 1, 1);
        }

        // once first move is found, check who starts first
        if (token == "1.") {
            blackStarts = false;
            break;
        }
        if (token == "1...") {
            blackStarts = true;
            break;
        }
    }
    return fen;
}

std::vector<std::string> getPositions(std::ifstream& file, std::string startFen, const WinningColor result, bool blackStarts) {
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

    Board board(startFen);
    BoardMove move;

    // if black is to move, do black's turn first before getting into the main loop for formatting reasons
    if (blackStarts) {
        std::string fen = board.toFen();
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
            fens.push_back(fen);
        }

        file >> token; // load the next move number
    }

    // read moves
    while (token != toStr(result)) {
        // two sides make moves
        for (int i = 0; i < 2; i++) {
            std::string fen = board.toFen();
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
                fens.push_back(fen);
            }
        }
        file >> token; // load the next move number
    }
    return fens;
}

BoardMove getMove(std::string input, Board& board) {
    MoveList gen(board);
    gen.generateAllMoves(board);
    std::vector<BoardMove> moves{};
    for (const auto move: gen.moves) {
        moves.push_back(move);
    }
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
            if (board.getPiece(move.sqr1()) == allyKing 
                && castleRightsBit(move.sqr2(), board.isWhiteTurn)
                && getFile(move.sqr2()) == castleFile) {
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
    } else {
        currPiece = board.isWhiteTurn ? WPawn : BPawn;
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
    Square dest = toSquare(input);

    // filter moves
    auto notCurrPiece = [board, currPiece](BoardMove move) {
        return board.getPiece(move.sqr1()) != currPiece;};
    if (currPiece != EmptyPiece) {
        moves.erase(std::remove_if(moves.begin(), moves.end(), notCurrPiece), moves.end());
    }

    auto notPromotePiece = [board, promotePiece](BoardMove move) {
        return move.getPromotePiece() != promotePiece;};
    if (promotePiece != EmptyPiece) {
        moves.erase(std::remove_if(moves.begin(), moves.end(), notPromotePiece), moves.end());
    }
    
    auto notRank = [board, rank](BoardMove move) {
        return getRank(move.sqr1()) != rank;};
    if (rank != -1) {
        moves.erase(std::remove_if(moves.begin(), moves.end(), notRank), moves.end());
    }
    
    auto notFile = [board, file](BoardMove move) {
        return getFile(move.sqr1()) != file;};
    if (file != -1) {
        moves.erase(std::remove_if(moves.begin(), moves.end(), notFile), moves.end());
    }

    auto notDest = [board, dest](BoardMove move) {
        return move.sqr2() != dest;
    };
    moves.erase(std::remove_if(moves.begin(), moves.end(), notDest), moves.end());

    if (moves.size() != 1) {
        std::cout << "Moves size: "<< moves.size() << std::endl;
        throw std::runtime_error("Move not selected for: " + origInput + " with fen " + board.toFen());
    }
    board.makeMove(moves[0]);
    return moves[0];
}

void storeFenResults(std::ofstream& file, std::vector<std::string> fens, WinningColor result) {
    if (result == NA) return;

    std::string resultStr = toStr(result);
    int storedPositions = 0;
    for (std::string fen: fens) {
        // only accept 1 / MOD_FENS portion of total fens
        if (!(Zobrist::rand64(seed) % MOD_FENS == 0)) {
            continue;
        }

        Board board(fen);

        // filter unwanted positions
        uint64_t nonPawns = board.pieceSets[WHITE_PIECES] | board.pieceSets[BLACK_PIECES];
        nonPawns ^= board.pieceSets[WPawn] | board.pieceSets[BPawn];
        if (popcount(nonPawns) <= 4) {
            continue;
        }

        file << fen << "; [" << resultStr << "]\n";
        ++storedPositions;
    }
}

std::string toStr(const WinningColor result) {
    if (result == WHITE) {return "1-0";}
    else if (result == DRAW) {return "1/2-1/2";}
    else if (result == BLACK) {return "0-1";}
    return "*";
}