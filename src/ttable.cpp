#include <cstdint>
#include <vector>

#include "ttable.hpp"
#include "move.hpp"

namespace TTable {

// global definition
TTable table = TTable();

void TTable::resize(int sizeMb) {
    // sizeof uses bytes and not megabytes
    int numElements = sizeMb * 1024 * 1024 / sizeof(Entry);
    this->table.resize(numElements);
}

void TTable::clear() {
    std::fill(this->table.begin(), this->table.end(), Entry());
}

bool TTable::entryExists(int index) {
    return this->table[index].key != 0;
}

Entry TTable::getEntry(int index) {
    return this->table[index];
}

void TTable::storeEntry(int key, Entry entry){
    this->table[key] = entry;
}

int TTable::getIndex(uint64_t key) {
    return key % this->size;
}

} // namespace TTable