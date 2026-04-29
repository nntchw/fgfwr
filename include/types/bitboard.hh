#pragma once

#include <bit>
#include <cstdint>

namespace fgfwr::types {

/// A bitboard (see below for more information).
/// Internally stored as an 64-bit unsigned integer.
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
/// - A bitboard is an ordered set of squares, with each square represented
/// by a single bit. A bit is set (1) if the square is included, and unset (0)
/// otherwise.
class Bitboard {
    uint64_t data;

  public:
    constexpr Bitboard() : data(0) {}

    constexpr Bitboard(uint64_t data) : data(data) {}

    [[nodiscard]] constexpr auto operator~() const -> Bitboard { return ~data; }
    [[nodiscard]] constexpr auto operator&(Bitboard rhs) const -> Bitboard {
        return data & rhs;
    }
    [[nodiscard]] constexpr auto operator|(Bitboard rhs) const -> Bitboard {
        return data | rhs;
    }
    [[nodiscard]] constexpr auto operator^(Bitboard rhs) const -> Bitboard {
        return data ^ rhs;
    }
    [[nodiscard]] constexpr auto operator<<(int rhs) const -> Bitboard {
        return data << rhs;
    }
    [[nodiscard]] constexpr auto operator>>(int rhs) const -> Bitboard {
        return data >> rhs;
    }

    constexpr void operator&=(Bitboard rhs) { data &= rhs; }
    constexpr void operator|=(Bitboard rhs) { data |= rhs; }
    constexpr void operator^=(Bitboard rhs) { data ^= rhs; }
    constexpr void operator<<=(int rhs) { data <<= rhs; }
    constexpr void operator>>=(int rhs) { data >>= rhs; }

    [[nodiscard]] constexpr auto operator+(Bitboard rhs) const -> Bitboard {
        return data + rhs;
    }
    [[nodiscard]] constexpr auto operator-(Bitboard rhs) const -> Bitboard {
        return data - rhs;
    }
    [[nodiscard]] constexpr auto operator*(Bitboard rhs) const -> Bitboard {
        return data * rhs;
    }

    constexpr void operator+=(Bitboard rhs) { data += rhs; }
    constexpr void operator-=(Bitboard rhs) { data -= rhs; }
    constexpr void operator*=(Bitboard rhs) { data *= rhs; }

    [[nodiscard]] constexpr auto operator-() const -> Bitboard { return -data; }

    [[nodiscard]] constexpr operator uint64_t() const { return data; }

    /// Returns the least significant bit of the bitboard.
    ///
    /// # Notes:
    /// - Calling this on an empty bitboard is undefined behavior.
    [[nodiscard]] constexpr auto lsb() const -> int {
        return std::countr_zero(data);
    }

    /// Returns the number of 1s on the bitboard.
    [[nodiscard]] constexpr auto count_ones() const -> int {
        return std::popcount(data);
    }

    constexpr void set(int index) { data |= 1ULL << index; }
    constexpr void unset(int index) { data &= ~(1ULL << index); }

    /// Return the least significant bit then remove it.
    [[nodiscard]] constexpr auto take_lsb() -> int {
        int l = lsb();
        data &= data - 1;
        return l;
    }

    /// Return a bitboard with every bit besides the index set to 0.
    ///
    /// Return an empty bitboard if the bit at index is 0.
    [[nodiscard]] constexpr auto get_bit(int index) const -> Bitboard {
        return data & (1ULL << index);
    }

    static const Bitboard EMPTY;
    static const Bitboard RANK_1;
    static const Bitboard RANK_2;
    static const Bitboard RANK_3;
    static const Bitboard RANK_4;
    static const Bitboard RANK_5;
    static const Bitboard RANK_6;
    static const Bitboard RANK_7;
    static const Bitboard RANK_8;

    static const Bitboard FILE_A;
    static const Bitboard FILE_B;
    static const Bitboard FILE_C;
    static const Bitboard FILE_D;
    static const Bitboard FILE_E;
    static const Bitboard FILE_F;
    static const Bitboard FILE_G;
    static const Bitboard FILE_H;
};

inline constexpr Bitboard Bitboard::EMPTY = 0x0;
inline constexpr Bitboard Bitboard::RANK_1 = 0xff00000000000000;
inline constexpr Bitboard Bitboard::RANK_2 = 0xff000000000000;
inline constexpr Bitboard Bitboard::RANK_3 = 0xff0000000000;
inline constexpr Bitboard Bitboard::RANK_4 = 0xff00000000;
inline constexpr Bitboard Bitboard::RANK_5 = 0xff000000;
inline constexpr Bitboard Bitboard::RANK_6 = 0xff0000;
inline constexpr Bitboard Bitboard::RANK_7 = 0xff00;
inline constexpr Bitboard Bitboard::RANK_8 = 0xff;

inline constexpr Bitboard Bitboard::FILE_A = 0x101010101010101;
inline constexpr Bitboard Bitboard::FILE_B = 0x202020202020202;
inline constexpr Bitboard Bitboard::FILE_C = 0x404040404040404;
inline constexpr Bitboard Bitboard::FILE_D = 0x808080808080808;
inline constexpr Bitboard Bitboard::FILE_E = 0x1010101010101010;
inline constexpr Bitboard Bitboard::FILE_F = 0x2020202020202020;
inline constexpr Bitboard Bitboard::FILE_G = 0x4040404040404040;
inline constexpr Bitboard Bitboard::FILE_H = 0x8080808080808080;

} // namespace fgfwr::types
