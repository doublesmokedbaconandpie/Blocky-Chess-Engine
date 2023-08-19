cmake_minimum_required(VERSION 3.14) 

project(Blocky)

add_executable(uci src/uci.cpp)

target_sources(uci PRIVATE
    src/board.cpp
    src/inCheck.cpp
    src/moveGen.cpp
    src/search.cpp
    src/eval.cpp
)

target_include_directories(uci PRIVATE
    src
)
