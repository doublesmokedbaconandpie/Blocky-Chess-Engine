#pragma once

#include <array>
#include <cstdint>

#include "utils/types.hpp"

class PieceSets {
    public:
        auto get(pieceTypes piece) const -> uint64_t;
        auto get(pieceTypes piece, bool isWhite) const -> uint64_t;

        auto view() const -> const std::array<uint64_t, NUM_BITBOARDS>&;

        auto operator[](std::size_t index) -> uint64_t&;
        auto operator[](std::size_t index) const -> const uint64_t&;
        auto operator==(const PieceSets& rhs) const -> bool;
    private:
        std::array<uint64_t, NUM_BITBOARDS> data;
};


inline auto PieceSets::view() const -> const std::array<uint64_t, NUM_BITBOARDS>& {
    return this->data;
}

inline auto PieceSets::operator[](std::size_t index) -> uint64_t& {
    return this->data[index];
}

inline auto PieceSets::operator[](std::size_t index) const -> const uint64_t& {
    return this->data[index];
}

inline auto PieceSets::operator==(const PieceSets& rhs) const -> bool {
    return this->view() == rhs.view();
}