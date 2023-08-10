#include <stdexcept>
#include <iostream>
#include "../src/board.hpp"

int testGetPiece1() {
    Board defaultChess = Board();
    int invalidRank = -1;
    int invalidFile = nullFile;
    pieceTypes invalidPiece = defaultChess.getPiece(invalidRank, invalidFile);
    if (invalidPiece != nullPiece) {
        std::invalid_argument("testGetPiece1 failed");
        return 0;
    }
    else {
        std::cout << "testGetPiece1 success" << std::endl;
    }
    return 1;
}

int main() {
    testGetPiece1();
    return 0;
}