#pragma once

#include "types/position.hh"
#include <string_view>

namespace fgfwr::fci {

/// FEN string of the starting position
inline constexpr std::string_view STARTPOS_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

/// Parses a FEN string into a `Position`.
///
/// The input must follow standard Forsyth–Edwards Notation (FEN).
/// Returns a fully initialized `Position` corresponding to the given FEN.
///
/// # Notes:
/// - Does not validate legality of the position (e.g. check, piece counts).
/// - Behavior is undefined if the FEN string is malformed.
auto parse_fen(std::string_view) -> types::Position;

} // namespace fgfwr::fci
