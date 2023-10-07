#include "uci.hpp"
#include "attacks.hpp"
#include "zobrist.hpp"

int main() {
    Zobrist::init();
    Attacks::init();
    Eval::init();

    if (!Uci::uci()) {return 1;}
    Uci::setOptionLoop();
    Uci::uciLoop();

    return 0;
}