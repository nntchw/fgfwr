#pragma once

#include <cstdint>

namespace fgfwr::types {

/// Player color (White, Black).
/// Internally stored as an 8-bit unsigned integer in the range [0, 1].
///
/// # Notes:
/// - The class performs no bound checking. Making sure the internal value stay
/// within in the valid range is the caller's responsibility.
class Color {
  public:
    enum Value : uint8_t { White, Black };

  private:
    Value data;

  public:
    /// Expects Color::White or Color::Black
    constexpr Color(Value color) : data(color) {};

    [[nodiscard]] constexpr operator uint8_t() const { return data; }

    [[nodiscard]] constexpr auto inverse() const -> Color {
        return static_cast<Value>(1 ^ data);
    }
};

} // namespace fgfwr::types
