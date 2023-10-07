# Blocky Chess Engine

## Overview

Blocky is a UCI-compliant chess engine. It has implemented the following techniques below thus far: 

* Alpha-beta search with fail-soft
* Magic Bitboards
* Zobrist-Hashing
* Piece-Square Tables

## Compiling Blocky

In order to get started running Blocky, first clone this repository. Then, use ```CMakeLists.txt``` to build this project. If you are using a Linux distribution, simply run:

```
cmake -S . -B build
cmake --build build
```

Similarly, if you want to build for Windows, minGW is required. If minGW is already installed, run the following:

```
cmake -S . -B build -DMAKE_EXE=ON
cmake --build build
```

Afterwards, the executable will be located within the ```build``` folder. 
