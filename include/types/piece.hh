#pragma once

#include <cstdint>

namespace fgfwr::types {

/// A chess piece.
///
/// Internally stored as an 8-bit unsigned integer in the range [0, 6].
///
/// # Notes:
/// - The class performs no bound checking. Making sure the internal value stay
/// within in the valid range is the caller's responsibility.
class Piece {
  public:
    enum Value : uint8_t {
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King,

        /// Sentinel value representing non-existent piece.
        ///
        /// Calling methods on this is undefined with the exception of
        /// `is_null()`.
        Null = 255
    };

  private:
    Value data;

  public:
    /// Expects Piece::Pawn / Piece::Knight / ...
    constexpr Piece(Value piece) : data(piece) {};

    [[nodiscard]] constexpr operator uint8_t() const { return data; }

    [[nodiscard]] constexpr auto is_null() const -> uint8_t {
        return data == Null;
    }
};

} // namespace fgfwr::types
