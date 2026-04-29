#pragma once

#include <cstdint>
#include <utility>

namespace fgfwr::types {

/// Bitmask encoding castling rights.
///
/// 8 4 2 1
/// x x x x
/// ▲ ▲ ▲ ▲
/// │ │ │ └── White kingside
/// │ │ └──── White queenside
/// │ └────── Black kingside
/// └──────── Black queenside
enum class CastlingMask {
    Empty = 0,
    WhiteShort = 1,
    WhiteLong = 2,
    BlackShort = 4,
    BlackLong = 8,

    White = WhiteShort | WhiteLong,
    Black = BlackShort | BlackLong,
    Short = WhiteShort | BlackShort,
    Long = WhiteLong | BlackLong,
};

/// Castling rights stored as 4-bit mask.
///
/// This type is intended to be treated as a value object:
/// - Construct and modify locally
/// - Then assign to a `Position` via `Position::set_cr`
class CastlingRights {
    uint8_t data{};

  public:
    [[nodiscard]] constexpr operator uint8_t() const { return data; }

    /// Enables the given castling right in this mask.
    ///
    /// This is intended for constructing a new `CastlingRights` value.
    /// Modifying castling rights already stored inside a `Position`
    /// should be done via `Position::set_cr`.
    void set_mask(CastlingMask mask) { data |= std::to_underlying(mask); }

    /// Disables the given castling right in this mask.
    ///
    /// This is intended for constructing a new `CastlingRights` value.
    /// Modifying castling rights already stored inside a `Position`
    /// should be done via `Position::set_cr`.
    void clear_mask(CastlingMask mask) { data &= ~std::to_underlying(mask); }

    /// Checks whether a specific castling right is present.
    [[nodiscard]] auto is_mask_set(CastlingMask mask) const -> bool {
        return data & std::to_underlying(mask);
    }
};

} // namespace fgfwr::types
