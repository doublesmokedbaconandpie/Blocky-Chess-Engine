cmake_minimum_required(VERSION 3.16)
project(Blocky VERSION 2.0.0)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(allTests
    testBitboard.cpp
    testBoard.cpp
    testEval.cpp
    testMoveGen.cpp
    testMoveOrder.cpp

    ../src/bitboard.cpp
    ../src/attacks.cpp
    ../src/move.cpp
    ../src/board.cpp
    ../src/moveGen.cpp
    ../src/moveOrder.cpp
    ../src/pieceSets.cpp
    ../src/timeman.cpp
    ../src/ttable.cpp
    ../src/search.cpp
    ../src/eval.cpp
)
target_include_directories(allTests PUBLIC "../src/")
target_compile_options(allTests PRIVATE -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Og)
target_link_libraries(allTests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(allTests)