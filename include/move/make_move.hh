#pragma once

#include "types/move.hh"
#include "types/position.hh"

namespace fgfwr::move {

/// Make a move in the position.
///
/// The move is assumed to be legal; no validation is performed.
/// The move should have been generated for this position
/// (via `fgfwr::move::generate_moves`).
void make_move(types::Position& pos, const types::Move& move);

} // namespace fgfwr::move
