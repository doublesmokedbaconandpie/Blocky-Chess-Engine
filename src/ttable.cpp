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

bool TTable::entryExists(uint64_t key) const {
    int index = this->getIndex(key);
    return static_cast<uint16_t>(key) == this->table[index].key;
}

Entry TTable::getEntry(int index) const {
    return this->table[index];
}

void TTable::storeEntry(int index, Entry entry) {
    this->table[index] = entry;
}

int TTable::getIndex(uint64_t key) const {
    return key % this->size;
}

} // namespace TTable