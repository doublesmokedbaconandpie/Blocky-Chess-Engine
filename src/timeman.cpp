#include "timeman.hpp"
#include <iostream>
namespace TIMEMAN {
    int timeToDepth(int ms) {
        // these values are completely arbitrary and only serve to be used to found development
        // for time management later on and prevent time outs for increasing depths
        if (ms >= 10000000) {return 10;}
        if (ms >= 1000000) {return 8;}
        if (ms >= 500000) {return 7;}
        if (ms >= 30000) {return 6;}
        if (ms >= 2000) {return 5;}
        if (ms >= 500) {return 4;}
        if (ms >= 100) {return 3;}
        if (ms >= 20) {return 2;}
        return 1;
    }
} // namespace TIMEMAN
