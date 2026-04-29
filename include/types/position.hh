#pragma once

#include "constants.hh"
#include "types/bitboard.hh"
#include "types/castling_rights.hh"
#include "types/color.hh"
#include "types/piece.hh"
#include "types/square.hh"
#include "zobrist.hh"

#include <array>
#include <cstdint>

namespace fgfwr::types {

/// A chess position.
///
/// This structures stores as much information as a FEN string with an addition
/// of a zobrist hash.
class Position {
    std::array<Bitboard, constants::COLOR_NB> color_bb{};
    std::array<Bitboard, constants::PIECE_TYPE_NB> piece_bb{};

    Color side_to_move = Color::White;
    CastlingRights castling_rights{};
    Square ep_square = Square::Null;
    uint8_t half_move_clock = 0;
    uint64_t hash = 0;

  public:
    Position() = default;

    Position(const Position&) = delete;
    auto operator=(const Position&) -> Position& = delete;

    Position(Position&&) = default;
    auto operator=(Position&&) -> Position& = default;

    /// Returns an identical copy of the position.
    [[nodiscard]] auto clone() const -> Position {
        Position p;
        p.color_bb = color_bb;
        p.piece_bb = piece_bb;
        p.castling_rights = castling_rights;
        p.side_to_move = side_to_move;
        p.ep_square = ep_square;
        p.half_move_clock = half_move_clock;
        return p;
    }

    /// Returns all squares occupied by the given piece type.
    [[nodiscard]] auto get_bb(Color color) const -> Bitboard {
        return color_bb[color];
    }
    /// Returns all squares occupied by the given color.
    [[nodiscard]] auto get_bb(Piece piece) const -> Bitboard {
        return piece_bb[piece];
    }
    /// Returns all squares occupied by the given color and piece type.
    [[nodiscard]] auto get_bb(Color color, Piece piece) const -> Bitboard {
        return color_bb[color] & piece_bb[piece];
    }

    /// Returns all squares occupied by a piece.
    [[nodiscard]] auto get_occupancy() const -> Bitboard {
        return color_bb[Color::White] | color_bb[Color::Black];
    }

    /// Returns the side to move.
    [[nodiscard]] auto get_stm() const -> Color { return side_to_move; }

    /// Returns castling rights for the given color.
    [[nodiscard]] auto get_cr() const -> CastlingRights {
        return castling_rights;
    }

    /// Returns the en passant target square, or `Square::None` if unavailable.
    [[nodiscard]] auto get_ep_square() const -> Square { return ep_square; }

    /// Returns the half-move clock (plies since last irreversible move).
    [[nodiscard]] auto get_hmc() const -> uint8_t { return half_move_clock; }

    /// Returns the Zobrist hash of the position.
    [[nodiscard]] auto get_hash() const -> uint64_t { return hash; }

    /// Returns the piece type of the piece on the given square.
    /// Returns `Piece::Null` if the square is empty.
    [[nodiscard]] auto piece_at(Square square) const -> Piece {
        for (int piece = Piece::Pawn; piece <= Piece::King; ++piece) {
            if (piece_bb[piece].get_bit(square))
                return static_cast<Piece::Value>(piece);
        }

        return Piece::Null;
    }

    /// Adds a piece to the board.
    void add_piece(Color color, Piece piece, Square square) {
        color_bb[color].set(square);
        piece_bb[piece].set(square);
        hash ^= zobrist::COLOR_BB_KEY[color][square];
        hash ^= zobrist::PIECE_BB_KEY[piece][square];
    }

    /// Removes a piece from the board.
    void clear_piece(Color color, Piece piece, Square square) {
        color_bb[color].unset(square);
        piece_bb[piece].unset(square);
        hash ^= zobrist::COLOR_BB_KEY[color][square];
        hash ^= zobrist::PIECE_BB_KEY[piece][square];
    }

    /// Set side to move to the given color.
    void set_stm(Color color) {
        if (color != side_to_move) {
            hash ^= zobrist::SIDE_TO_MOVE_KEY;
        }
        side_to_move = color;
    }

    /// Changes side to move.
    void switch_side() {
        side_to_move = side_to_move.inverse();
        hash ^= zobrist::SIDE_TO_MOVE_KEY;
    }

    /// Sets castling rights.
    ///
    /// Requires creating a new `CastlingRights` object.
    void set_cr(CastlingRights rights) {
        hash ^= zobrist::CASTLING_RIGHTS_KEYS[castling_rights];
        castling_rights = rights;
        hash ^= zobrist::CASTLING_RIGHTS_KEYS[castling_rights];
    }

    /// Sets new en passant square. Accepts `Square::Null` to mark en
    /// passant as unavailable.
    void set_ep_square(Square square) {
        if (!ep_square.is_null()) {
            hash ^= zobrist::EP_SQUARE_KEYS[ep_square];
        }

        ep_square = square;

        if (!ep_square.is_null()) {
            hash ^= zobrist::EP_SQUARE_KEYS[ep_square];
        }
    }

    /// Increments half-move clock
    void increment_hmc() { ++half_move_clock; }
    /// Sets half-move clock to 0.
    void reset_hmc() { half_move_clock = 0; }
    /// Sets half-move clock to the specified value. Only really useful when
    /// constructing position from FEN.
    void set_hmc(uint8_t hmc) { half_move_clock = hmc; }
};

} // namespace fgfwr::types
