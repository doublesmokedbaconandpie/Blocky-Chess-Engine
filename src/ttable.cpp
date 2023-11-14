#include <cstdint>
#include <iostream>
#include <vector>

#include "bitboard.hpp"
#include "ttable.hpp"
#include "move.hpp"

namespace TTable {

// global definition
TTable table = TTable();

void TTable::resize(int sizeMb) {
    // sizeof uses bytes and not megabytes
    this->size = sizeMb * 1024 * 1024 / sizeof(Entry);
    this->table.resize(this->size);
}

void TTable::clear() {
    std::fill(this->table.begin(), this->table.end(), Entry());
}

// estimates how much the table is full in tenths of percents
int TTable::hashFull() {
    int entriesUsed = 0;
    for (int i = 0; i < 1000; ++i) {
        if (this->table[i].move != BoardMove()) {
            ++entriesUsed;
        }
    }
    return entriesUsed;
}

bool TTable::entryExists(uint64_t key) const {
    int index = this->getIndex(key);
    return key == this->table[index].key;
}

Entry TTable::getEntry(uint64_t key) const {
    return this->table[this->getIndex(key)];
}

void TTable::storeEntry(uint64_t key, Entry entry) {
    this->table[this->getIndex(key)] = entry;
}

int TTable::getIndex(uint64_t key) const {
    return key % this->size;
}

} // namespace TTable