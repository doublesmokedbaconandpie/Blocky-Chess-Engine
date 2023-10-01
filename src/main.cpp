#include "uci.hpp"
#include "zobrist.hpp"

int main() {
    Zobrist::init();
    Eval::init();

    if (!Uci::uci()) {return 1;}
    Uci::setOptionLoop();
    Uci::uciLoop();

    return 0;
}