#include "uci.hpp"

int main() {
    if (!UCI::uci()) {return 1;}
    UCI::SETOPTIONLOOP();
    UCI::UCILOOP();

    return 0;
}
