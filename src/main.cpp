#include "uci.hpp"
#include "attacks.hpp"
#include "zobrist.hpp"

int main() {
    Attacks::init();

    Uci::printEngineInfo();
    Uci::setOptionLoop();
    Uci::uciLoop();

    return 0;
}