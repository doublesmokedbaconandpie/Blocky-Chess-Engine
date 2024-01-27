/*
* Blocky, a UCI chess engine
* Copyright (C) 2023-2024, Kevin Nguyen
*
* Blocky is free software; you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
*
* Blocky is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along with this program; 
* if not, see <https://www.gnu.org/licenses>.
*/

#include <cstdint>
#include <iostream>
#include <vector>

#include "bitboard.hpp"
#include "ttable.hpp"
#include "move.hpp"

namespace TTable {

// global definition
TTable Table = TTable();

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
    const int index = this->getIndex(key);
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