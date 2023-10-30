cmake_minimum_required(VERSION 3.16) 
project(Blocky)
add_executable(extract extract.cpp)

target_sources(extract PRIVATE
    ../../src/moveGen.cpp
    ../../src/board.cpp
    ../../src/move.cpp
    ../../src/zobrist.cpp
    ../../src/eval.cpp
    ../../src/attacks.cpp
    ../../src/bitboard.cpp
)
target_include_directories(extract PUBLIC "../../src/")
target_compile_options(extract PRIVATE -O3)
