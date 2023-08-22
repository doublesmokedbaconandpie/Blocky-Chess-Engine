#include "timeman.hpp"

int negamaxDepthBasedOnTime(int ms) {
    if (ms >= 30000) {return 4;}
    if (ms >= 10000) {return 3;}
    if (ms >= 2000) {return 2;}
    return 1;
}