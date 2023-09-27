#pragma once

#include "board.hpp"

//Note
//  
//Some more eval related variables can have their operations moved to setPiece


static const std::array<int, BOARD_SIZE> tableKingOp = {
    0 , -30, -30, -30, -30, -30, -30, 0 ,
    0 , -30, -30, -30, -30, -30, -30, 0 ,
    0 , -30, -30, -30, -30, -30, -30, 0 ,
    0 , -20, -30, -50, -50, -30, -20, 0 ,
    0 , -20, -30, -50, -50, -30, -20, 0 ,
    0 , -15, -30, -30, -30, -30, -15, 0 ,
    10, -10, -15, -15, -15, -15, -15, 10,
    20, 20, 10, 10 , 10, 10, 20, 20 
};

//needs a bit of work
static const std::array<int, BOARD_SIZE> tableQueenOp = {
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 ,
    0 , 0 , 3 , 3 , 3 , 3 , 0 , 0 ,
    0 , 3 , 3 , 3 , 3 , 3 , 3 , 0 ,
    0 , 3 , 3 , 3 , 3 , 3 , 3 , 0 ,
    0 , 5 , -3, 5 , 5 , -3, 5 , 0 ,
    0 , 2 , 2 , 1 , 1 , 2 , 2 , 0 ,
    -9, -4, -4, 2 , 2 , -4, -4, -9
};

static const std::array<int, BOARD_SIZE> tableBishopOp = {
    -20, -10, -5, -5, -5, -5, -10, -20,
    -10, -5, 5 , 5 , 5 , 5 , -5, -10,
    -5, 5 , 10, 15, 15, 10, 5 , -5,
    -5, 5 , 15, 15, 15, 15, 5 , -5,
    -5, 5 , 15, 15, 15, 15, 5 , -5,
    -5, 15, 15, 15, 15, 15, 15, -5,
    -10, 10, 5 , 5 , 5 , 5 , 10, -10,
    -20, -5, -10, -5, -5, -10, -5, -20,
};

static const std::array<int, BOARD_SIZE> tableKnightOp = {
    -20, -10, -5, -5, -5, -5, -10, -20,
    -10, -5, 0 , 5 , 5 , 0 , -5, -10,
    -5, 5 , 15, 15, 15, 15, 5 , -5,
    -5, 5 , 15, 18, 18, 15, 5 , -5,
    -5, 5 , 15, 18, 18, 15, 5 , -5,
    -5, 5 , 15, 15, 15, 15, 5 , -5,
    -10, 0, 5 , 5 , 5 , 5 , 0, -10,
    -25, -20, -10, -10, -10, -10, -20, -25,
};

static const std::array<int, BOARD_SIZE> tableRookOp = {
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    20, 20, 20, 20, 20, 20, 20, 20,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 5 , 5 , 5 , 5 , 5 , 5 , 0 ,
    0 , 0 , 10, 10, 10, 10, 0 , 0 
};

static const std::array<int, BOARD_SIZE> tablePawnOp = {
    99, 99, 99, 99, 99, 99, 99, 99,
    20, 20, 25, 30, 30, 25, 20, 20,
    10, 10, 15, 25, 25, 15, 10, 10,
    0 , 5 , 10, 28, 28, 10, 5 , 0 ,
    0 , 0 , 6 , 19, 19, 6 , 0 , 0 , 
    0 , 5 , 3 , 5 , 5 , 3 , 5 , 0 ,
    0 , 7 , 0 , -10, -10, 0 , 7,  0 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 
};

static const std::array<std::array<int, BOARD_SIZE>, 6> tablesOp = 
{tableKingOp, tableQueenOp, tableBishopOp, tableKnightOp, tableRookOp, tablePawnOp};

static const std::array<int, BOARD_SIZE> tableKingEg = {
    -20, -15, -7, 0 , 0 , -7, -15, -20,
    -15, -7, 0 , 5 , 5 , 0 , -7, -15,
    -7, 0 , 5 , 15, 15, 5 , 0 , -7,
    0 , 5 , 15, 18, 18, 15, 5 , 0 ,
    0 , 5 , 15, 18, 18, 15, 5 , 0 ,
    -7, 0 , 5 , 15, 15, 5 , 0 , -7,
    -15, -7, 0 , 5 , 5 , 0 , -7, -15,
    -20, -15, -7, 0 , 0 , -7, -15, -20 
};

static const std::array<int, BOARD_SIZE> tableQueenEg = {
    -5, -5, -5, -5, -5, -5, -5, -5,
    -5, 0 , 0 , 0 , 0 , 0 , 0 , -5,
    -5, 0 , 5 , 5 , 5 , 5 , 0 , -5,
    -5, 0 , 5 , 15, 15, 5 , 0 , -5,
    -5, 0 , 5 , 15, 15, 5 , 0 , -5,
    -5, 0 , 5 , 5 , 5 , 5 , 0 , -5,
    -5, 0 , 0 , 0 , 0 , 0 , 0 , -5,
    -5, -5, -5, -5, -5, -5, -5, -5
};

//slightly modified from opening
static const std::array<int, BOARD_SIZE> tableBishopEg = {
    -10, -7, -5, -5, -5, -5, -7, -10,
    -7, 3, 4 , 5 , 5 , 4 , 3, -7,
    -5, 4 , 10, 15, 15, 10, 4 , -5,
    -5, 5 , 15, 15, 15, 15, 5 , -5,
    -5, 5 , 15, 15, 15, 15, 5 , -5,
    -5, 4 , 10, 15, 15, 10, 4 , -5,
    -7, 3 , 4 , 5 , 5 , 4 , 3 , -7,
    -10, -7, -5, -5, -5, -5, -7, -10,
};

//copied from opening for now
static const std::array<int, BOARD_SIZE> tableKnightEg = {
    -20, -10, -5, -5, -5, -5, -10, -20,
    -10, -5, 0 , 5 , 5 , 0 , -5, -10,
    -5, 5 , 15, 15, 15, 15, 5 , -5,
    -5, 5 , 15, 18, 18, 15, 5 , -5,
    -5, 5 , 15, 18, 18, 15, 5 , -5,
    -5, 5 , 15, 15, 15, 15, 5 , -5,
    -10, 0, 5 , 5 , 5 , 5 , 0, -10,
    -25, -20, -10, -10, -10, -10, -20, -25,
};

//idk what to do here
static const std::array<int, BOARD_SIZE> tableRookEg = {
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 
};

static const std::array<int, BOARD_SIZE> tablePawnEg = {
    99, 99, 99, 99, 99, 99, 99, 99,
    120, 110, 100, 100, 100, 100, 110, 120,
    80, 75, 71, 71, 71, 71, 75, 80,
    50, 47, 44, 44, 44, 44, 47, 50,
    31, 29, 27, 27, 27, 27, 29, 31, 
    12, 11, 10, 10, 10, 10, 11, 12,
    2 , 2 , 2 , 2 , 2 , 2 , 2,  2 , 
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
};

static const std::array<std::array<int, BOARD_SIZE>, 6> tablesEg = 
{tableKingEg, tableQueenEg, tableBishopEg, tableKnightEg, tableRookEg, tablePawnEg};



static const std::array<std::array<std::array<int, BOARD_SIZE>, 6>, 2> tables = {tablesOp, tablesEg}; //this is space inefficient but just trying to get it working for now

int getPlacementScore(int rank, int file, pieceTypes currPiece, gameProgress gameState);

int eval(Board board);