#pragma once

#include "types/piece.hh"
#include "types/square.hh"

#include <cstdint>
#include <tuple>
#include <utility>

namespace fgfwr::types {

/// 4-bit move flag indicating move type.
///
/// 8 4 2 1
/// x x x x
/// ▲ ▲ ▲ ▲
/// │ │ │ │
/// │ │ └─┴── reserved for special moves
/// │ └────── capture
/// └──────── promotion
enum class MoveFlag : uint8_t {
    Normal,
    DoublePush,
    ShortCastle,
    LongCastle,
    Capture,
    EnPassant,
    KnightPromo = 8,
    BishopPromo,
    RookPromo,
    QueenPromo,
    KnightPromoCapture,
    BishopPromoCapture,
    RookPromoCapture,
    QueenPromoCapture
};

/// A chess move.
///
/// # Representation:
/// - `from` : source square
/// - `to` : destination square
/// - `flag` : move type and special properties (see `MoveFlag`)
/// - `piece` : moving piece type
struct Move {
    Square from = Square::Null;
    Square to = Square::Null;
    MoveFlag flag = MoveFlag::Normal;
    Piece piece = Piece::Null;

    [[nodiscard]] auto is_capture() const -> bool {
        return std::to_underlying(flag) & 4;
    }
    [[nodiscard]] auto is_promotion() const -> bool {
        return std::to_underlying(flag) & 8;
    }

    /// Returns the piece type this pawn promotes to.
    ///
    /// Does not check if the move itself is actually a promotion.
    [[nodiscard]] auto promote_to() const -> Piece {
        return static_cast<Piece::Value>((std::to_underlying(flag) & 3) + 1);
    }

    [[nodiscard]] auto is_null() const -> uint8_t { return to.is_null(); }

    /// Packs the move into a 16-bit integer for compact storage (e.g.
    /// transposition table).
    ///
    /// # Encoding:
    /// - bits [0–5]   : from
    /// - bits [6–11]  : to
    /// - bits [12–15] : flag
    ///
    /// Information about moving piece type is lost.
    ///
    /// This function must not be used with `Move::Null`.
    /// Null move is not uniquely representable in packed form,
    /// and unpacking will not reconstruct a null move.
    [[nodiscard]] auto pack() const -> uint16_t {
        return from | (to << 6) | (std::to_underlying(flag) << 12);
    }

    /// Unpacks a 16-bit move into [from, to, flag].
    [[nodiscard]] static auto unpack(uint16_t packed_move)
        -> std::tuple<Square, Square, MoveFlag> {
        return {packed_move & 63,
                packed_move >> 6 & 63,
                static_cast<MoveFlag>(packed_move >> 12)};
    }

    /// Sentinel value representing non-existent move.
    ///
    /// Calling methods on this is undefined with the exception of `is_null()`.
    static const Move Null;
};

inline constexpr Move Move::Null = {.from = Square::Null,
                                    .to = Square::Null,
                                    .flag = MoveFlag::Normal,
                                    .piece = Piece::Null};

} // namespace fgfwr::types
