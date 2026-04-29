#include "move/movegen.hh"

#include "constants.hh"
#include "move/lookups.hh"
#include "types/bitboard.hh"
#include "types/castling_rights.hh"
#include "types/color.hh"
#include "types/move.hh"
#include "types/move_list.hh"
#include "types/piece.hh"
#include "types/position.hh"
#include "types/square.hh"

#include <array>

using namespace fgfwr::constants;
using namespace fgfwr::types;

void fgfwr::move::generate_moves(MoveList& move_list, const Position& pos) {
    const Color stm = pos.get_stm();
    const Color not_stm = stm.inverse();
    const Bitboard occ = pos.get_occupancy();
    const Bitboard allies = pos.get_bb(stm);
    const Bitboard enemies = pos.get_bb(not_stm);

    auto emit_moves =
        [&](const Square from, Bitboard attacks, const Piece piece) -> void {
        // No self-capture!
        attacks &= ~allies;
        while (attacks) {
            const Square to = attacks.take_lsb();
            const bool is_capture = enemies.get_bit(to);
            move_list.push_back(
                {.from = from,
                 .to = to,
                 .flag = is_capture ? MoveFlag::Capture : MoveFlag::Normal,
                 .piece = piece});
        }
    };

    // Knight
    {
        Bitboard knights = pos.get_bb(stm, Piece::Knight);
        while (knights) {
            Square from = knights.take_lsb();
            Bitboard attacks = get_knight_attack(from);
            emit_moves(from, attacks, Piece::Knight);
        }
    }

    // Bishop
    {
        Bitboard bishops = pos.get_bb(stm, Piece::Bishop);
        while (bishops) {
            Square from = bishops.take_lsb();
            Bitboard attacks = get_bishop_attack(from, occ);
            emit_moves(from, attacks, Piece::Bishop);
        }
    }

    // Rook
    {
        Bitboard rooks = pos.get_bb(stm, Piece::Rook);
        while (rooks) {
            Square from = rooks.take_lsb();
            Bitboard attacks = get_rook_attack(from, occ);
            emit_moves(from, attacks, Piece::Rook);
        }
    }

    // Queen
    {
        Bitboard queens = pos.get_bb(stm, Piece::Queen);
        while (queens) {
            Square from = queens.take_lsb();
            Bitboard attacks = get_queen_attack(from, occ);
            emit_moves(from, attacks, Piece::Queen);
        }
    }

    // King
    {
        // We never have more than one king, so we don't need to iterate.
        Square from = pos.get_bb(stm, Piece::King).lsb();
        Bitboard attacks = get_king_attack(from);
        emit_moves(from, attacks, Piece::King);
    }

    // Castling
    {
        struct CastlingParams {
            Square king_from;
            Square king_to_short;
            Square king_to_long;
            Square rook_from_short;
            Square rook_from_long;
            CastlingMask short_mask;
            CastlingMask long_mask;
        };

        static constexpr std::array<CastlingParams, COLOR_NB> CASTLING_PARAMS =
            {{{
                  .king_from = Square::E1,
                  .king_to_short = Square::G1,
                  .king_to_long = Square::C1,
                  .rook_from_short = Square::H1,
                  .rook_from_long = Square::A1,
                  .short_mask = CastlingMask::WhiteShort,
                  .long_mask = CastlingMask::WhiteLong,
              },
              {
                  .king_from = Square::E8,
                  .king_to_short = Square::G8,
                  .king_to_long = Square::C8,
                  .rook_from_short = Square::H8,
                  .rook_from_long = Square::A8,
                  .short_mask = CastlingMask::BlackShort,
                  .long_mask = CastlingMask::BlackLong,
              }}};

        const auto cr = pos.get_cr();
        const auto& [king_from,
                     king_to_short,
                     king_to_long,
                     rook_from_short,
                     rook_from_long,
                     short_mask,
                     long_mask] = CASTLING_PARAMS[stm];

        if (cr.is_mask_set(short_mask)) {
            const auto path = get_squares_between(king_from, rook_from_short);
            if (!(path & occ)) {
                move_list.push_back({.from = king_from,
                                     .to = king_to_short,
                                     .flag = MoveFlag::ShortCastle,
                                     .piece = Piece::King});
            }
        }

        if (cr.is_mask_set(long_mask)) {
            const auto path = get_squares_between(king_from, rook_from_long);
            if (!(path & occ)) {
                move_list.push_back({.from = king_from,
                                     .to = king_to_long,
                                     .flag = MoveFlag::LongCastle,
                                     .piece = Piece::King});
            }
        }
    }

    // Pawn
    {
        struct PawnPushParams {
            Bitboard third_rank;
            Bitboard promo_rank;
            int push_offset;
        };

        static constexpr std::array<PawnPushParams, COLOR_NB>

            PAWN_PUSH_PARAMS = {{{
                                     .third_rank = Bitboard::RANK_3,
                                     .promo_rank = Bitboard::RANK_8,
                                     .push_offset = -RANK_OFFSET,
                                 },
                                 {
                                     .third_rank = Bitboard::RANK_6,
                                     .promo_rank = Bitboard::RANK_1,
                                     .push_offset = RANK_OFFSET,
                                 }}};

        const auto& [third_rank, promo_rank, push_offset] =
            PAWN_PUSH_PARAMS[stm];

        Bitboard pawns = pos.get_bb(stm, Piece::Pawn);
        const Bitboard single_pushed =
            (push_offset < 0 ? pawns >> RANK_OFFSET : pawns << RANK_OFFSET) &
            ~occ;
        Bitboard normal_pushed = single_pushed & ~promo_rank;
        Bitboard promo_pushed = single_pushed & promo_rank;
        // If a pawn can be pushed to the third rank unobstructed, it must be
        // eligible for a double push.
        Bitboard double_pushed =
            (push_offset < 0 ? (third_rank & single_pushed) >> RANK_OFFSET
                             : (third_rank & single_pushed) << RANK_OFFSET) &
            ~occ;

        while (normal_pushed) {
            const Square to = normal_pushed.take_lsb();
            move_list.push_back({.from = to - push_offset,
                                 .to = to,
                                 .flag = MoveFlag::Normal,
                                 .piece = Piece::Pawn});
        }

        while (promo_pushed) {
            const Square to = promo_pushed.take_lsb();
            const Square from = to - push_offset;
            move_list.push_back({.from = from,
                                 .to = to,
                                 .flag = MoveFlag::KnightPromo,
                                 .piece = Piece::Pawn});
            move_list.push_back({.from = from,
                                 .to = to,
                                 .flag = MoveFlag::BishopPromo,
                                 .piece = Piece::Pawn});
            move_list.push_back({.from = from,
                                 .to = to,
                                 .flag = MoveFlag::RookPromo,
                                 .piece = Piece::Pawn});
            move_list.push_back({.from = from,
                                 .to = to,
                                 .flag = MoveFlag::QueenPromo,
                                 .piece = Piece::Pawn});
        }

        while (double_pushed) {
            const Square to = double_pushed.take_lsb();
            move_list.push_back({.from = to - (2 * push_offset),
                                 .to = to,
                                 .flag = MoveFlag::DoublePush,
                                 .piece = Piece::Pawn});
        }

        // captures
        {
            while (pawns) {
                Square from = pawns.take_lsb();
                Bitboard attacks = get_pawn_attack(stm, from);
                attacks &= enemies;
                if (attacks & promo_rank) {
                    while (attacks) {
                        const Square to = attacks.take_lsb();
                        move_list.push_back(
                            {.from = from,
                             .to = to,
                             .flag = MoveFlag::KnightPromoCapture,
                             .piece = Piece::Pawn});
                        move_list.push_back(
                            {.from = from,
                             .to = to,
                             .flag = MoveFlag::BishopPromoCapture,
                             .piece = Piece::Pawn});
                        move_list.push_back({.from = from,
                                             .to = to,
                                             .flag = MoveFlag::RookPromoCapture,
                                             .piece = Piece::Pawn});
                        move_list.push_back(
                            {.from = from,
                             .to = to,
                             .flag = MoveFlag::QueenPromoCapture,
                             .piece = Piece::Pawn});
                    }

                } else {
                    while (attacks) {
                        const Square to = attacks.take_lsb();
                        move_list.push_back({.from = from,
                                             .to = to,
                                             .flag = MoveFlag::Capture,
                                             .piece = Piece::Pawn});
                    }
                }
            }

            const Square ep_square = pos.get_ep_square();
            if (!ep_square.is_null()) {
                Bitboard en_passant_pawns =
                    get_pawn_attack(not_stm, ep_square) & allies;
                while (en_passant_pawns) {
                    const Square from = en_passant_pawns.take_lsb();
                    move_list.push_back({.from = from,
                                         .to = ep_square,
                                         .flag = MoveFlag::EnPassant,
                                         .piece = Piece::Pawn});
                }
            }
        }
    }
}
