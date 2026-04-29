#pragma once

#include "types/bitboard.hh"

#include <cstdint>

namespace fgfwr::types {

/// A square on chessboard represented as an index into a bitboard
///
/// Internally stored as an 8-bit unsigned integer in the range [0, 63].
///
/// Squares are indexed in row-major order, from top to bottom
/// and left to right.
///
/// In other words:
/// a8 = 0,  ..., h8 = 7,
/// ... ,
/// a1 = 56, ..., h1 = 63.
///
/// # Notes:
/// - The class performs no bound checking. Making sure the internal value stay
/// within in the valid range is the caller's responsibility.
class Square {
    uint8_t data;

  public:
    constexpr Square(uint8_t data) : data(data) {}

    [[nodiscard]] constexpr auto operator+(int rhs) const -> Square {
        return data + rhs;
    }
    [[nodiscard]] constexpr auto operator-(int rhs) const -> Square {
        return data - rhs;
    }

    constexpr void operator+=(int rhs) { data += rhs; }
    constexpr void operator-=(int rhs) { data -= rhs; }

    constexpr auto operator++() -> Square& {
        ++data;
        return *this;
    }
    constexpr auto operator--() -> Square& {
        --data;
        return *this;
    }

    constexpr auto operator++(int) -> Square {
        auto temp = *this;
        ++data;
        return temp;
    }
    constexpr auto operator--(int) -> Square {
        auto temp = *this;
        --data;
        return temp;
    }

    [[nodiscard]] constexpr operator uint8_t() const { return data; }

    /// Returns a bitboard with only this square set.
    [[nodiscard]] constexpr auto to_bb() const -> Bitboard {
        return 1ULL << data;
    }

    /// Returns the rank index [0–7], where 0 corresponds to rank 1.
    [[nodiscard]] constexpr auto rank() const -> int { return 7 - (data >> 3); }
    /// Returns the file index [0–7], where 0 corresponds to A file.
    [[nodiscard]] constexpr auto file() const -> int { return data & 7; }

    [[nodiscard]] constexpr auto is_null() const -> bool {
        return data == Null;
    }

    // clang-format off
    enum : uint8_t {
        A8, B8, C8, D8, E8, F8, G8, H8,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A1, B1, C1, D1, E1, F1, G1, H1,

        /// Sentinel value representing non-existent square.
        ///
        /// Calling methods on this is undefined with the exception of
        /// `is_null()`.
        Null = 255,
    };
    // clang-format on
};

} // namespace fgfwr::types
