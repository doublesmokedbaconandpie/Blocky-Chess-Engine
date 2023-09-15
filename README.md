# Blocky Chess Engine

Blocky is a UCI-compliant chess engine. Given that Blocky is currently a new engine, there aren't too many features and techniques yet to make this engine strong. The following are the ones that we have implemented: 

* Alpha-beta search with fail-soft
* Bitboards
* Zobrist-Hashing

## Building the Engine

In order to get started modifying and running Blocky, first clone this repository. Then, use the CMake files to build this project. If you are using a Linux distribution, simply run:

```
cmake -S . -B build -DMAKE_EXE=ON
cmake --build build
```

Afterwards, the executable will be located within the build folder. Similarly, if you want to build for Windows, run the following:

```
cmake -S . -B build -DMAKE_EXE=ON
cmake --build build
```
