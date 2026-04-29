#pragma once

namespace fgfwr::constants {

/// Number of squares on the board.
inline constexpr int SQUARE_NB = 64;
/// Number of colors.
inline constexpr int COLOR_NB = 2;
/// Number of piece types.
inline constexpr int PIECE_TYPE_NB = 6;
/// Maximum number of moves in a position.
inline constexpr int MAX_MOVE_NB = 218;
/// Number of different permutations of castling rights
inline constexpr int CASTLING_NB = 16;
/// Offset between two adjacent squares on the same file
/// Note that because the board representation is indexed top-down, the offset
/// is inverted.
///
/// e.g. Square::A7 + RANK_OFFSET = Square::A6
inline constexpr int RANK_OFFSET = 8;
/// Offset between two adjacent squares on the same file
///
/// e.g. Square::A7 + FILE_OFFSET = Square::B7
inline constexpr int FILE_OFFSET = 1;

} // namespace fgfwr::constants
