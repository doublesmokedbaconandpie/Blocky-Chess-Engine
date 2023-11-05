#pragma once

#include <cstdint>
#include <vector>

#include "move.hpp"

namespace TTable {

constexpr uint64_t KEY_MASK = 0xFFFFull;
constexpr int DEFAULT_SIZEMB = 128;

struct Entry {
    uint16_t key = 0;
    int depth = 0;
    BoardMove move = BoardMove();
};

class TTable {
    public:
        void resize(int sizeMb);
        TTable() {this->resize(DEFAULT_SIZEMB);};
        void clear();

        bool entryExists(int index);
        Entry getEntry(int index);
        void storeEntry(int index, Entry entry);
        int getIndex(uint64_t key);
    private:
        std::vector<Entry> table;
        int size;
};

// global declaration
extern TTable table;

} // namespace TTable