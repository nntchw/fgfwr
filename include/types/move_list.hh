#pragma once

#include "constants.hh"
#include "types/move.hh"

#include <array>
#include <cstddef>
#include <utility>

namespace fgfwr::types {

/// A fixed-capacity, array-backed container for storing chess moves.
class MoveList {
    std::array<Move, constants::MAX_MOVE_NB> data{};
    size_t length = 0;

  public:
    MoveList() = default;

    MoveList(const MoveList&) = delete;
    auto operator=(const MoveList&) -> MoveList& = delete;

    [[nodiscard]] auto operator[](size_t index) -> Move& { return data[index]; }

    [[nodiscard]] auto begin() -> Move* const { return data.begin(); }

    [[nodiscard]] auto end() -> Move* const { return data.begin() + length; }

    /// Returns the number of stored moves
    [[nodiscard]] auto size() const -> int { return length; }

    /// Appends a move to the end of the list.
    ///
    /// Does not check for capacity overflow.
    void push_back(Move item) { data[length++] = item; }

    /// Removes the last move.
    ///
    /// The list must not be empty.
    void pop_back() { --length; }

    /// Removes all moves from the list.
    ///
    /// Does not modify underlying storage.
    void clear() { length = 0; }

    /// Swaps two moves at the given indices.
    ///
    /// No bounds checking is performed.
    void swap(int index_a, int index_b) {
        std::swap(data[index_a], data[index_b]);
    };

    /// Removes and returns the last move.
    ///
    /// The list must not be empty.
    [[nodiscard]] auto take_last() -> Move { return data[--length]; };
};

} // namespace fgfwr::types
