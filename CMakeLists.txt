cmake_minimum_required(VERSION 3.14)  # You can adjust the minimum required version

project(Blocky)

# Add the executable target
add_executable(uci src/uci.cpp)

# Add source files to the executable target
target_sources(uci PRIVATE
    # List all your source files here
    src/board.cpp
    src/inCheck.cpp
    src/moveGen.cpp
    src/search.cpp
    src/eval.cpp
)

# Add header files to the target's include directories
target_include_directories(uci PRIVATE
    # List all your include directories here
    src
)
