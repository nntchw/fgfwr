#pragma once

#include "types/move_list.hh"
#include "types/position.hh"

namespace fgfwr::move {

/// Populate the given move list with all legal moves in the given position.
void generate_moves(types::MoveList& move_list, const types::Position& pos);

} // namespace fgfwr::move
