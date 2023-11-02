cmake_minimum_required(VERSION 3.16) 
project(Blocky)
add_executable(Magic magic.cpp)

target_sources(Magic PRIVATE
    ../src/attacks.cpp
    ../src/zobrist.cpp
    ../src/bitboard.cpp
)
target_include_directories(Magic PUBLIC "../src/")
target_compile_options(Magic PRIVATE -O3)