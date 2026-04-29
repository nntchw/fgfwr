#include "move/make_move.hh"

#include "constants.hh"
#include "types/castling_rights.hh"
#include "types/color.hh"
#include "types/move.hh"
#include "types/piece.hh"
#include "types/position.hh"
#include "types/square.hh"
#include <array>

using namespace fgfwr::constants;
using namespace fgfwr::types;

void fgfwr::move::make_move(types::Position& pos, const types::Move& move) {
    const auto& [from, to, flag, piece] = move;
    const auto stm = pos.get_stm();
    const auto not_stm = stm.inverse();

    // Manage the pieces 'indirectly' affected by the move.
    if (move.flag <= MoveFlag::DoublePush) {
        // nothing to do here
    } else if (move.is_capture()) {
        if (move.flag == MoveFlag::EnPassant) {
            // The captured pawn is be right behind the en passant square.
            const Square captured_sq =
                to - (stm == Color::White ? RANK_OFFSET : -RANK_OFFSET);
            pos.clear_piece(not_stm, Piece::Pawn, captured_sq);
        } else {
            const Piece captured = pos.piece_at(to);
            pos.clear_piece(not_stm, captured, to);
        }
    } else if (move.flag == MoveFlag::ShortCastle) {
        // King lands on G-file, rook moves from H-file to F-file.
        // . . . . K . . R
        // . . . . . R K .
        pos.clear_piece(stm, Piece::Rook, to + FILE_OFFSET);
        pos.add_piece(stm, Piece::Rook, to - FILE_OFFSET);
    } else if (move.flag == MoveFlag::LongCastle) {
        // King lands on C-file, rook moves from A-file to D-file.
        // R . . . K . . .
        // . . K R . . . .
        pos.clear_piece(stm, Piece::Rook, to - (2 * FILE_OFFSET));
        pos.add_piece(stm, Piece::Rook, to + FILE_OFFSET);
    }

    if (piece == Piece::Pawn or move.is_capture()) {
        pos.reset_hmc();
    } else {
        pos.increment_hmc();
    }

    // Move the piece itself.
    pos.clear_piece(stm, piece, from);
    pos.add_piece(stm, move.is_promotion() ? move.promote_to() : piece, to);

    // Update castling rights.
    static constexpr auto SQUARE_CR_MASK = []() -> auto {
        std::array<CastlingMask, SQUARE_NB> table{};
        table.fill(CastlingMask::Empty);

        table[Square::A1] = CastlingMask::WhiteLong;
        table[Square::H1] = CastlingMask::WhiteShort;
        table[Square::A8] = CastlingMask::BlackLong;
        table[Square::H8] = CastlingMask::BlackShort;

        table[Square::E1] = CastlingMask::White;
        table[Square::E8] = CastlingMask::Black;

        return table;
    }();

    auto new_cr = pos.get_cr();
    new_cr.clear_mask(SQUARE_CR_MASK[from]);
    new_cr.clear_mask(SQUARE_CR_MASK[to]);
    pos.set_cr(new_cr);

    // Expire en passant square and set a new one if necessary.
    pos.set_ep_square(Square::Null);
    if (move.flag == MoveFlag::DoublePush) {
        pos.set_ep_square(move.to -
                          (stm == Color::White ? RANK_OFFSET : -RANK_OFFSET));
    }

    pos.switch_side();
}
