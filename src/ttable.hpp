#pragma once

#include <cstdint>
#include <vector>

#include "move.hpp"

namespace TTable {

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

        int getIndex(uint64_t key) const;
        bool entryExists(uint64_t key) const;
        Entry getEntry(int index) const;
        void storeEntry(int index, Entry entry);
        std::vector<Entry> table;
    private:
        int size;
};

// global declaration
extern TTable table;

} // namespace TTable