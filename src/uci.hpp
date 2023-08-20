#include <string>
#include <sstream>

#include "board.hpp"

int main();

namespace UCI {
    struct UCIOPTIONS {
        int depth = 3;
    };

    bool uci();

    void SETOPTIONLOOP();
    void setoption(std::istringstream& input);

    void UCILOOP();
    Board position(std::istringstream& input);
    void go(std::istringstream& input, Board& board);

    void isready();

} // namespace UCI
