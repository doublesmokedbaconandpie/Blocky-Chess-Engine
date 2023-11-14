#pragma once

#include <cstdint>
#include <vector>

#include "move.hpp"

namespace TTable {

constexpr int DEFAULT_SIZEMB = 128;

struct Entry {
    uint64_t key = 0;
    uint8_t age = 0;
    int depth = 0;
    BoardMove move = BoardMove();
};

class TTable {
    public:
        void resize(int sizeMb);
        TTable() {this->resize(DEFAULT_SIZEMB);};
        void clear();
        int hashFull();

        int getIndex(uint64_t key) const;
        bool entryExists(uint64_t key) const;
        Entry getEntry(int index) const;
        void storeEntry(int index, Entry entry);
    private:
        std::vector<Entry> table;
        int size;
};

// global declaration
extern TTable table;

} // namespace TTable