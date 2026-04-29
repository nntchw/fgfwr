#include "move/lookups.hh"

#include "constants.hh"
#include "move/magic.hh"
#include "types/bitboard.hh"
#include "types/color.hh"
#include "types/square.hh"

#include <array>
#include <vector>

using namespace fgfwr::types;
using namespace fgfwr::constants;

///   A  B  C  D  E  F  G  H
/// 1 .  .  .  .  .  .  .  .
/// 2 .  .  .  .  .  .  .  .
/// 3 .  .  9  .  7  .  .  .
/// 4 .  .  .  p  .  .  .  .
/// 5 .  .  7  .  9  .  .  .
/// 6 .  .  .  .  .  .  .  .
/// 7 .  .  .  .  .  .  .  .
/// 8 .  .  .  .  .  .  .  .
consteval auto init_pawn_attacks() -> auto {
    std::array<std::array<Bitboard, SQUARE_NB>, COLOR_NB> table;
    for (Square from = 0; from < SQUARE_NB; from += 1) {
        Bitboard from_bb = from.to_bb();

        table[Color::White][from] = ((from_bb >> 9) & ~Bitboard::FILE_H) |
                                    ((from_bb >> 7) & ~Bitboard::FILE_A);
        table[Color::Black][from] = ((from_bb << 9) & ~Bitboard::FILE_A) |
                                    ((from_bb << 7) & ~Bitboard::FILE_H);
    }

    return table;
}

///   A  B  C  D  E  F  G  H
/// 1 .  .  .  .  .  .  .  .
/// 2 .  .  17 .  15 .  .  .
/// 3 .  10 .  .  .  6  .  .
/// 4 .  .  .  n  .  .  .  .
/// 5 .  6  .  .  .  10 .  .
/// 6 .  .  15 .  17 .  .  .
/// 7 .  .  .  .  .  .  .  .
/// 8 .  .  .  .  .  .  .  .
consteval auto init_knight_attacks() -> auto {
    std::array<Bitboard, SQUARE_NB> table;
    for (Square from = 0; from < SQUARE_NB; from += 1) {
        Bitboard from_bb = from.to_bb();

        table[from] =
            ((from_bb >> 17) & ~Bitboard::FILE_H) |
            ((from_bb << 17) & ~Bitboard::FILE_A) |
            ((from_bb >> 15) & ~Bitboard::FILE_A) |
            ((from_bb << 15) & ~Bitboard::FILE_H) |
            ((from_bb >> 10) & ~Bitboard::FILE_H & ~Bitboard::FILE_G) |
            ((from_bb << 10) & ~Bitboard::FILE_A & ~Bitboard::FILE_B) |
            ((from_bb >> 6) & ~Bitboard::FILE_A & ~Bitboard::FILE_B) |
            ((from_bb << 6) & ~Bitboard::FILE_H & ~Bitboard::FILE_G);
    }
    return table;
}

///   A  B  C  D  E  F  G  H
/// 1 .  .  .  .  .  .  .  .
/// 2 .  .  .  .  .  .  .  .
/// 3 .  .  9  8  7  .  .  .
/// 4 .  .  1  k  1  .  .  .
/// 5 .  .  7  8  9  .  .  .
/// 6 .  .  .  .  .  .  .  .
/// 7 .  .  .  .  .  .  .  .
/// 8 .  .  .  .  .  .  .  .
consteval auto init_king_attacks() -> auto {
    std::array<Bitboard, SQUARE_NB> table;
    for (Square from = 0; from < SQUARE_NB; from += 1) {
        Bitboard from_bb = from.to_bb();

        table[from] = (from_bb >> 8) | (from_bb << 8) |
                      ((from_bb >> 9) & ~Bitboard::FILE_H) |
                      ((from_bb << 9) & ~Bitboard::FILE_A) |
                      ((from_bb >> 7) & ~Bitboard::FILE_A) |
                      ((from_bb << 7) & ~Bitboard::FILE_H) |
                      ((from_bb >> 1) & ~Bitboard::FILE_H) |
                      ((from_bb << 1) & ~Bitboard::FILE_A);
    }
    return table;
}

enum {
    North = -8,
    South = 8,
    East = 1,
    West = -1,
    Northeast = -7,
    Northwest = -9,
    Southeast = 9,
    Southwest = 7
};

consteval auto
gen_sliding_attack(Square from, Bitboard occ, int shift, Bitboard acc)
    -> Bitboard {
    if (((shift == East or shift == Northeast or shift == Southeast) and
         from.file() >= 7) or
        ((shift == West or shift == Northwest or shift == Southwest) and
         from.file() <= 0) or
        ((shift == North or shift == Northeast or shift == Northwest) and
         from.rank() >= 7) or
        ((shift == South or shift == Southeast or shift == Southwest) and
         from.rank() <= 0) or
        (acc & occ) != 0)
        return acc;

    from += shift;
    acc.set(from);

    return acc | gen_sliding_attack(from, occ, shift, acc);
}

consteval auto gen_bishop_attack(Square from, Bitboard occ) -> Bitboard {
    return gen_sliding_attack(from, occ, Northeast, Bitboard::EMPTY) |
           gen_sliding_attack(from, occ, Northwest, Bitboard::EMPTY) |
           gen_sliding_attack(from, occ, Southeast, Bitboard::EMPTY) |
           gen_sliding_attack(from, occ, Southwest, Bitboard::EMPTY);
}

consteval auto gen_rook_attack(Square from, Bitboard occ) -> Bitboard {
    return gen_sliding_attack(from, occ, North, Bitboard::EMPTY) |
           gen_sliding_attack(from, occ, South, Bitboard::EMPTY) |
           gen_sliding_attack(from, occ, East, Bitboard::EMPTY) |
           gen_sliding_attack(from, occ, West, Bitboard::EMPTY);
}

consteval auto powerset(Bitboard bb) -> std::vector<Bitboard> {
    std::vector<Bitboard> subsets;

    Bitboard mask = 0;
    do {
        subsets.emplace_back(mask);

        mask = (mask - bb) & bb;
    } while (mask);

    return subsets;
};

consteval auto
fill_table(Square square, auto& entry, auto& table, auto* generator) -> auto {
    for (auto occ : powerset(entry.mask)) {
        table[entry.magic_index(occ)] = generator(square, occ);
    }
};

consteval auto init_bishop_attacks() -> auto {
    std::array<std::array<Bitboard, 512>, SQUARE_NB> table;
    for (int from = 0; from < SQUARE_NB; ++from) {
        fill_table(from,
                   fgfwr::move::BISHOP_MAGICS[from],
                   table[from],
                   gen_bishop_attack);
    }

    return table;
}

consteval auto init_rook_attacks() -> auto {
    std::array<std::array<Bitboard, 4096>, SQUARE_NB> table;
    for (int from = 0; from < SQUARE_NB; ++from) {
        fill_table(from,
                   fgfwr::move::ROOK_MAGICS[from],
                   table[from],
                   gen_rook_attack);
    }

    return table;
}

consteval auto init_squares_betweens()
    -> std::array<std::array<Bitboard, SQUARE_NB>, SQUARE_NB> {
    std::array<std::array<Bitboard, SQUARE_NB>, SQUARE_NB> table;
    for (Square square_a = 0; square_a < SQUARE_NB; square_a += 1) {
        for (Square square_b = 0; square_b < SQUARE_NB; square_b += 1) {
            Bitboard between_ab = square_a.to_bb() | square_b.to_bb();

            int rank_sq_a = square_a.rank();
            int rank_sq_b = square_b.rank();

            int file_sq_a = square_a.file();
            int file_sq_b = square_b.file();

            int diag_sq_a = 7 + rank_sq_a - file_sq_a;
            int diag_sq_b = 7 + rank_sq_b - file_sq_b;

            int antidiag_sq_a = rank_sq_a + file_sq_a;
            int antidiag_sq_b = rank_sq_b + file_sq_b;

            if (diag_sq_a == diag_sq_b or antidiag_sq_a == antidiag_sq_b) {
                table[square_a][square_b] =
                    gen_bishop_attack(square_a, between_ab) &
                    gen_bishop_attack(square_b, between_ab);
            }

            if (file_sq_a == file_sq_b or rank_sq_a == rank_sq_b) {
                table[square_a][square_b] =
                    gen_rook_attack(square_a, between_ab) &
                    gen_rook_attack(square_b, between_ab);
            }

            if (square_a == square_b) {
                table[square_a][square_b] = Bitboard::EMPTY;
            }
        }
    }
    return table;
}

static constexpr auto PAWN_ATTACKS = init_pawn_attacks();
static constexpr auto KNIGHT_ATTACKS = init_knight_attacks();
static constexpr auto KING_ATTACKS = init_king_attacks();
static constexpr auto BISHOP_ATTACKS = init_bishop_attacks();
static constexpr auto ROOK_ATTACKS = init_rook_attacks();
static constexpr auto SQUARES_BETWEENS = init_squares_betweens();

auto fgfwr::move::get_pawn_attack(Color color, Square from) -> Bitboard {
    return PAWN_ATTACKS[color][from];
}

auto fgfwr::move::get_knight_attack(Square from) -> Bitboard {
    return KNIGHT_ATTACKS[from];
}

auto fgfwr::move::get_king_attack(Square from) -> Bitboard {
    return KING_ATTACKS[from];
}

auto fgfwr::move::get_bishop_attack(Square from, Bitboard occ) -> Bitboard {
    return BISHOP_ATTACKS[from][BISHOP_MAGICS[from].magic_index(occ)];
}

auto fgfwr::move::get_rook_attack(Square from, Bitboard occ) -> Bitboard {
    return ROOK_ATTACKS[from][ROOK_MAGICS[from].magic_index(occ)];
}

auto fgfwr::move::get_queen_attack(Square from, Bitboard occ) -> Bitboard {
    return get_bishop_attack(from, occ) | get_rook_attack(from, occ);
}

auto fgfwr::move::get_squares_between(Square square_a, Square square_b)
    -> Bitboard {
    return SQUARES_BETWEENS[square_a][square_b];
}
