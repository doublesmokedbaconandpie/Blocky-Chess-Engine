# Blocky Chess Engine

## Overview

Blocky is a UCI-compliant chess engine. It has implemented the following techniques below thus far: 

* Search:
    * Alpha-Beta Search with fail-soft
    * Principle Variation Search
    * Null Move Pruning
    * Transposition Tables Cutoffs
* Evaluation: 
    * Piece-Square Tables
    * Tapered Evaluation
* Move Ordering: 
    * Most Valuable Victim - Least Valuable Attacker
    * Principle Variation Moves
* Other Techniques:
    * Magic Bitboards and Attack Tables
    * Zobrist-Hashing

## Compiling Blocky

In order to get started running Blocky, first clone this repository. Then, use ```CMakeLists.txt``` to build this project. CMake projects like this one can be built like so:

```
cmake -S . -B build
cmake --build build
```

As part of the development process used by us, Windows binaries may be compiled from WSL using the modified command below:

```
cmake -S . -B build -DMAKE_EXE=ON
cmake --build build
```

In both cases, the binary will be located within the ```build``` folder. 
