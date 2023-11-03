cmake_minimum_required(VERSION 3.16) 
project(Blocky)
set(CMAKE_CXX_STANDARD 20)
add_executable(Blocky src/main.cpp)

# Allow Windows binaries to be made from WSL
option(MAKE_EXE "exe?" OFF)
if(MAKE_EXE)
    message("Compiling for Windows EXE")
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
endif(MAKE_EXE)
unset(MAKE_EXE CACHE)

target_sources(Blocky PRIVATE
    src/zobrist.cpp
    src/bitboard.cpp
    src/attacks.cpp
    src/move.cpp
    src/board.cpp
    src/moveGen.cpp
    src/movePicker.cpp
    src/search.cpp
    src/eval.cpp
    src/timeman.cpp
    src/uci.cpp
)
target_include_directories(Blocky PRIVATE src)
target_compile_options(Blocky PRIVATE -O3)