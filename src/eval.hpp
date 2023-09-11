#pragma once

#include "board.hpp"

//Note
//  
//Some more eval related variables can have their operations moved to setPiece


//greatly unfinished, needs to add negative values for exposed king (towards center)
static const std::array<int, BOARD_SIZE> tableKingOp = {
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 20, 20, 5 , 10, 10, 20, 20 
};

//needs a lot of work
static const std::array<int, BOARD_SIZE> tableQueenOp = {
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 8 , 8 , 0 , 0 , 0 ,
    0 , 0 , 5 , 10, 10, 5 , 0 , 0 ,
    0 , 5 , 6 , 5 , 5 , 6 , 5 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , -4, -4, 0 , 0 , 0 
};

//Just copied knight table for testing, this should be changed
static const std::array<int, BOARD_SIZE> tableBishopOp = {
    -20, -10, -5, -5, -5, -5, -10, -20,
    -10, -5, 0 , 5 , 5 , 0 , -5, -10,
    -5, 0 , 0 , 15, 15, 0 , 0 , -5,
    -5, 0 , 15, 25, 25, 15, 0 , -5,
    -5, 0 , 15, 25, 25, 15, 0 , -5,
    -5, 5 , 10, 15, 15, 10, 5 , -5,
    -10, -5, 0 , 5 , 5 , 0 , -5, -10,
    -20, -10, -5, -5, -5, -5, -10, -20,
};

static const std::array<int, BOARD_SIZE> tableKnightOp = {
    -20, -10, -5, -5, -5, -5, -10, -20,
    -10, -5, 0 , 5 , 5 , 0 , -5, -10,
    -5, 0 , 0 , 15, 15, 0 , 0 , -5,
    -5, 0 , 15, 25, 25, 15, 0 , -5,
    -5, 0 , 15, 25, 25, 15, 0 , -5,
    -5, 5 , 10, 15, 15, 10, 5 , -5,
    -10, -5, 0 , 5 , 5 , 0 , -5, -10,
    -20, -10, -5, -5, -5, -5, -10, -20,
};

static const std::array<int, BOARD_SIZE> tableRookOp = {
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    20, 20, 20, 20, 20, 20, 20, 20,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 10, 10, 10, 0 , 0 
};

static const std::array<int, BOARD_SIZE> tablePawnOp = {
    99, 99, 99, 99, 99, 99, 99, 99,
    20, 20, 25, 30, 30, 25, 20, 20,
    10, 10, 15, 25, 25, 15, 10, 10,
    0 , 5 , 10, 20, 20, 10, 5 , 0 ,
    0 , 0 , 6 , 15, 15, 6 , 0 , 0 , 
    0 , 5 , 3 , 5 , 5 , 3 , 5 , 0 ,
    0 , 7 , 0 , -5, -5, 0 , 7,  0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 
};

static const std::array<std::array<int, BOARD_SIZE>, 6> tablesOp = 
{tableKingOp, tableQueenOp, tableBishopOp, tableKnightOp, tableRookOp, tablePawnOp};

int getPlacementScore(int rank, int file, pieceTypes currPiece);

int eval(Board board);