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

#pragma once

#include <cstdint>
#include <vector>

#include "move.hpp"
#include "utils/enums.hpp"

namespace TTable {

constexpr int DEFAULT_SIZEMB = 128;

struct Entry {
    uint64_t key{};
    uint8_t age{};
    int depth{};
    int eval{};
    EvalType bound = NONE;
    BoardMove move{};
};

class TTable {
    public:
        void resize(int sizeMb);
        TTable() {this->resize(DEFAULT_SIZEMB);};
        void clear();
        int hashFull();

        bool entryExists(uint64_t key) const;
        Entry getEntry(uint64_t key) const;
        void store(int eval, BoardMove move, EvalType bound, int depth, int age, uint64_t key);
    private:
        int getIndex(uint64_t key) const;
        std::vector<Entry> table;
        int size;
};

// global declaration
extern TTable Table;

} // namespace TTable