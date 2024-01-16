#include "uci.hpp"
#include "search.hpp"
#include "attacks.hpp"

int main() {
    Attacks::init();
    Search::initLMRTable();

    Uci::printEngineInfo();
    Uci::setOptionLoop();
    Uci::uciLoop();

    return 0;
}