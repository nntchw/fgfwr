#pragma once

#include "types/bitboard.hh"
#include "types/color.hh"
#include "types/square.hh"

namespace fgfwr::move {

/// Returns a bitboard of all squares attacked by a pawn of the given color
/// from the specified square.
///
/// This includes only capture moves, not forward pushes.
auto get_pawn_attack(types::Color color, types::Square from) -> types::Bitboard;

/// Returns a bitboard of all squares attacked by a knight from the
/// specified square.
auto get_knight_attack(types::Square from) -> types::Bitboard;

/// Returns a bitboard of all squares attacked by a king from the
/// specified square.
auto get_king_attack(types::Square from) -> types::Bitboard;

/// Returns a bitboard of all squares attacked by a bishop from the
/// specified square in the given set of occupied square.
///
/// Assumes all occupied squares contain enemy pieces.
auto get_bishop_attack(types::Square from, types::Bitboard occ)
    -> types::Bitboard;

/// Returns a bitboard of all squares attacked by a rook from the
/// specified square in the given set of occupied square.
///
/// Assumes all occupied squares contain enemy pieces.
auto get_rook_attack(types::Square from, types::Bitboard occ)
    -> types::Bitboard;

/// Returns a bitboard of all squares attacked by a queen from the
/// specified square in the given set of occupied square.
///
/// Assumes all occupied squares contain enemy pieces.
auto get_queen_attack(types::Square from, types::Bitboard occ)
    -> types::Bitboard;

/// Returns a bitboard of squares between 2 squares.
///
/// If the squares are aligned along a rank, file, or diagonal, the result
/// includes all squares between them, including both endpoints. Otherwise,
/// returns an empty bitboard.
auto get_squares_between(types::Square square_a, types::Square square_b)
    -> types::Bitboard;

} // namespace fgfwr::move
