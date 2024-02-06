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

#include <array>
#include <cstddef>

// internally uses a fixed size array to avoid memory reallocations, like what can happen in std::vector
// bounds checking is not supported
template<typename T, std::size_t N>
class FixedVector {
    public:
        // writing
        constexpr T& operator[](std::size_t index) {
            return this->container[index];
        }
        // reading
        constexpr const T& operator[](std::size_t index) const {
            return this->container[index];
        }
        constexpr void push_back(const T& entry) {
            this->pointer++;
            this->container[this->pointer] = entry;
        }
        constexpr void pop_back() {
            // there is no need to null initialize the old memory since push_back is the only valid way to access it
            this->pointer--;
        }
        constexpr T back() {
            return this->container[this->pointer];
        }
        constexpr auto begin() const {
            return this->container.begin();
        }
        constexpr auto end() const {
            return this->container.begin() + this->pointer + 1;
        }
        constexpr auto size() const {
            return this->pointer + 1;
        }
        constexpr auto clear() {
            // there is no need to null initialize the old memory since push_back is the only valid way to access it
            this->pointer = -1;
        }
        constexpr bool operator==(const FixedVector<T, N>& rhs) const {
            if (this->size() != rhs.size()) {
                return false;
            }
            for (std::size_t i = 0; i < this->size(); ++i) {
                if (this->container[i] != rhs[i]) {
                    return false;
                }
            }
            return true;
        }
    private:
        std::array<T, N> container{};
        std::size_t pointer = -1;
};
