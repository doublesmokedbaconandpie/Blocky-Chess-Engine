#include "uci.hpp"
#include "attacks.hpp"
#include "zobrist.hpp"

int main() {
    Attacks::init();
    Eval::init();

    Uci::printEngineInfo();
    Uci::setOptionLoop();
    Uci::uciLoop();

    return 0;
}