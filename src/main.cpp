#include "uci.hpp"
#include "zobrist.hpp"

int main() {
    if (!Uci::uci()) {return 1;}

    Zobrist::init();
    Uci::setOptionLoop();
    Uci::uciLoop();

    return 0;
}