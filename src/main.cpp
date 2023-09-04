#include "uci.hpp"

int main() {
    if (!Uci::uci()) {return 1;}
    Uci::setOptionLoop();
    Uci::uciLoop();

    return 0;
}