#pragma once

#include <cstdint>
#include <string>
#include <vector>

// benchmarking is used to test for any functional changes in the search
namespace Bench {

constexpr int BENCHDEPTH = 8;

// some fens from CPW
const std::vector<std::string> fens = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
    "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1",
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
    "8/8/1k1K4/8/2BN2p1/8/4P3/8 w - - 0 1",
    "rn1qkb1r/p4ppp/b1p1pn2/1p1p4/1P1P4/P1N1PN2/2P2PPP/R1BQKB1R w KQkq - 2 7",
    "r1b1k3/2q5/2p5/5p2/p1B2P1p/2P5/4QP2/4K2R b Kq - 0 28",
    "r1b5/6k1/2p5/4Pp2/p1B4R/2P5/5P2/4K3 w - - 1 32",
    "r1bqkb1r/pp1p1ppp/2n2n2/2p1p1B1/2P5/2NP1N2/PP2PPPP/R2QKB1R b KQkq - 5 5",
    "4rrk1/p1p2p2/6p1/q1p4p/2P5/4P1PP/P4PQ1/2RR2K1 w - - 0 22",
    "2r3k1/8/5P1R/2Nb4/1R1K4/1P5P/1r6/8 b - - 0 37",
    "6rk/1PP5/8/p2p4/4P1pp/4pP1P/8/1R4K1 w - - 1 38",
    "rn1qkb1r/1pp1pp1p/p2p1np1/8/2BPP1b1/P1N2N2/1PP2PPP/R1BQK2R b KQkq - 5 6",
    "2r2rk1/pp1bbpp1/1n2pn1p/q7/3P3B/1BN2N2/PPQ2PPP/3R1RK1 w - - 5 15",
    "rnr3k1/p3qpp1/1p2b2p/1Bpp4/3P4/Q3PN2/PP3PPP/2R1K2R b K - 3 14",
    "8/p2bk2p/1p3pp1/2pp4/5PP1/2PPKN1P/P1P5/8 b - - 5 35",
};

uint64_t start();

} // namespace Bench
