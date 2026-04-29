#include "move/make_move.hh"
#include "move/movegen.hh"
#include "types/move_list.hh"
#include "uci/fen.hh"
#include <print>

auto main(int argc, char* argv[]) -> int {
    auto pos =
        fgfwr::uci::parse_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

    constexpr char const* SQUARE_NAME[64]{"a8", "b8", "c8", "d8", "e8", "f8",
                                          "g8", "h8", "a7", "b7", "c7", "d7",
                                          "e7", "f7", "g7", "h7", "a6", "b6",
                                          "c6", "d6", "e6", "f6", "g6", "h6",
                                          "a5", "b5", "c5", "d5", "e5", "f5",
                                          "g5", "h5", "a4", "b4", "c4", "d4",
                                          "e4", "f4", "g4", "h4", "a3", "b3",
                                          "c3", "d3", "e3", "f3", "g3", "h3",
                                          "a2", "b2", "c2", "d2", "e2", "f2",
                                          "g2", "h2", "a1", "b1", "c1", "d1",
                                          "e1", "f1", "g1", "h1"};

    fgfwr::types::MoveList ml;
    fgfwr::move::generate_moves(ml, pos);
    std::println("{}", pos.get_hash());
    for (auto move : ml) {
        std::println("{}{}", SQUARE_NAME[move.from], SQUARE_NAME[move.to]);
    }
    fgfwr::move::make_move(pos, ml.take_last());

    fgfwr::types::MoveList ml2;
    fgfwr::move::generate_moves(ml2, pos);
    std::println("{}", pos.get_hash());
    for (auto move : ml2) {
        std::println("{}{}", SQUARE_NAME[move.from], SQUARE_NAME[move.to]);
    }
}
