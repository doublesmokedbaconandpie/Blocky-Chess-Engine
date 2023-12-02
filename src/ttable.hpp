#pragma once

#include <cstdint>
#include <vector>

#include "move.hpp"
#include "types.hpp"

namespace TTable {

constexpr int DEFAULT_SIZEMB = 128;

struct Entry {
    uint64_t key = 0;
    uint8_t age = 0;
    int depth = 0;
    int eval = 0;
    EvalType flag = NONE;
    BoardMove move;
};

class TTable {
    public:
        void resize(int sizeMb);
        TTable() {this->resize(DEFAULT_SIZEMB);};
        void clear();
        int hashFull();

        bool entryExists(uint64_t key) const;
        Entry getEntry(uint64_t key) const;
        void storeEntry(uint64_t key, Entry entry);
        int getIndex(uint64_t key) const;
    private:
        std::vector<Entry> table;
        int size;
};

// global declaration
extern TTable table;

} // namespace TTable