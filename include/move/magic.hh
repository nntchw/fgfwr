#pragma once

#include "constants.hh"
#include "types/bitboard.hh"

#include <array>
#include <cstdint>

namespace fgfwr::move {

struct MagicEntry {
    types::Bitboard mask;
    uint64_t magic{};
    int shift{};

    [[nodiscard]] constexpr auto magic_index(types::Bitboard occ) const -> int {
        occ &= mask;
        occ *= magic;
        occ >>= shift;

        return occ;
    }
};

inline constexpr std::array<MagicEntry, constants::SQUARE_NB> BISHOP_MAGICS = {{
    {.mask = 0x40201008040200, .magic = 0xffedf9fd7cfcffff, .shift = 58},
    {.mask = 0x402010080400, .magic = 0xfc0962854a77f576, .shift = 59},
    {.mask = 0x4020100a00, .magic = 0x5822022042000000, .shift = 59},
    {.mask = 0x40221400, .magic = 0x2ca804a100200020, .shift = 59},
    {.mask = 0x2442800, .magic = 0x204042200000900, .shift = 59},
    {.mask = 0x204085000, .magic = 0x2002121024000002, .shift = 59},
    {.mask = 0x20408102000, .magic = 0xfc0a66c64a7ef576, .shift = 59},
    {.mask = 0x2040810204000, .magic = 0x7ffdfdfcbd79ffff, .shift = 58},
    {.mask = 0x20100804020000, .magic = 0xfc0846a64a34fff6, .shift = 59},
    {.mask = 0x40201008040000, .magic = 0xfc087a874a3cf7f6, .shift = 59},
    {.mask = 0x4020100a0000, .magic = 0x1001080204002100, .shift = 59},
    {.mask = 0x4022140000, .magic = 0x1810080489021800, .shift = 59},
    {.mask = 0x244280000, .magic = 0x62040420010a00, .shift = 59},
    {.mask = 0x20408500000, .magic = 0x5028043004300020, .shift = 59},
    {.mask = 0x2040810200000, .magic = 0xfc0864ae59b4ff76, .shift = 59},
    {.mask = 0x4081020400000, .magic = 0x3c0860af4b35ff76, .shift = 59},
    {.mask = 0x10080402000200, .magic = 0x73c01af56cf4cffb, .shift = 59},
    {.mask = 0x20100804000400, .magic = 0x41a01cfad64aaffc, .shift = 59},
    {.mask = 0x4020100a000a00, .magic = 0x40c0422080a0598, .shift = 57},
    {.mask = 0x402214001400, .magic = 0x4228020082004050, .shift = 57},
    {.mask = 0x24428002800, .magic = 0x200800400e00100, .shift = 57},
    {.mask = 0x2040850005000, .magic = 0x20b001230021040, .shift = 57},
    {.mask = 0x4081020002000, .magic = 0x7c0c028f5b34ff76, .shift = 59},
    {.mask = 0x8102040004000, .magic = 0xfc0a028e5ab4df76, .shift = 59},
    {.mask = 0x8040200020400, .magic = 0x20208050a42180, .shift = 59},
    {.mask = 0x10080400040800, .magic = 0x1004804b280200, .shift = 59},
    {.mask = 0x20100a000a1000, .magic = 0x2048020024040010, .shift = 57},
    {.mask = 0x40221400142200, .magic = 0x102c04004010200, .shift = 55},
    {.mask = 0x2442800284400, .magic = 0x20408204c002010, .shift = 55},
    {.mask = 0x4085000500800, .magic = 0x2411100020080c1, .shift = 57},
    {.mask = 0x8102000201000, .magic = 0x102a008084042100, .shift = 59},
    {.mask = 0x10204000402000, .magic = 0x941030000a09846, .shift = 59},
    {.mask = 0x4020002040800, .magic = 0x244100800400200, .shift = 59},
    {.mask = 0x8040004081000, .magic = 0x4000901010080696, .shift = 59},
    {.mask = 0x100a000a102000, .magic = 0x280404180020, .shift = 57},
    {.mask = 0x22140014224000, .magic = 0x800042008240100, .shift = 55},
    {.mask = 0x44280028440200, .magic = 0x220008400088020, .shift = 55},
    {.mask = 0x8500050080400, .magic = 0x4020182000904c9, .shift = 57},
    {.mask = 0x10200020100800, .magic = 0x23010400020600, .shift = 59},
    {.mask = 0x20400040201000, .magic = 0x41040020110302, .shift = 59},
    {.mask = 0x2000204081000, .magic = 0xdcefd9b54bfcc09f, .shift = 59},
    {.mask = 0x4000408102000, .magic = 0xf95ffa765afd602b, .shift = 59},
    {.mask = 0xa000a10204000, .magic = 0x1401210240484800, .shift = 57},
    {.mask = 0x14001422400000, .magic = 0x22244208010080, .shift = 57},
    {.mask = 0x28002844020000, .magic = 0x1105040104000210, .shift = 57},
    {.mask = 0x50005008040200, .magic = 0x2040088800c40081, .shift = 57},
    {.mask = 0x20002010080400, .magic = 0x43ff9a5cf4ca0c01, .shift = 59},
    {.mask = 0x40004020100800, .magic = 0x4bffcd8e7c587601, .shift = 59},
    {.mask = 0x20408102000, .magic = 0xfc0ff2865334f576, .shift = 59},
    {.mask = 0x40810204000, .magic = 0xfc0bf6ce5924f576, .shift = 59},
    {.mask = 0xa1020400000, .magic = 0x80000b0401040402, .shift = 59},
    {.mask = 0x142240000000, .magic = 0x20004821880a00, .shift = 59},
    {.mask = 0x284402000000, .magic = 0x8200002022440100, .shift = 59},
    {.mask = 0x500804020000, .magic = 0x9431801010068, .shift = 59},
    {.mask = 0x201008040200, .magic = 0xc3ffb7dc36ca8c89, .shift = 59},
    {.mask = 0x402010080400, .magic = 0xc3ff8a54f4ca2c89, .shift = 59},
    {.mask = 0x2040810204000, .magic = 0xfffffcfcfd79edff, .shift = 58},
    {.mask = 0x4081020400000, .magic = 0xfc0863fccb147576, .shift = 59},
    {.mask = 0xa102040000000, .magic = 0x40c000022013020, .shift = 59},
    {.mask = 0x14224000000000, .magic = 0x2000104000420600, .shift = 59},
    {.mask = 0x28440200000000, .magic = 0x400000260142410, .shift = 59},
    {.mask = 0x50080402000000, .magic = 0x800633408100500, .shift = 59},
    {.mask = 0x20100804020000, .magic = 0xfc087e8e4bb2f736, .shift = 59},
    {.mask = 0x40201008040200, .magic = 0x43ff9e4ef4ca2c89, .shift = 58},
}};

inline constexpr std::array<MagicEntry, constants::SQUARE_NB> ROOK_MAGICS = {{
    {.mask = 0x101010101017e, .magic = 0xa180022080400230, .shift = 52},
    {.mask = 0x202020202027c, .magic = 0x40100040022000, .shift = 53},
    {.mask = 0x404040404047a, .magic = 0x80088020001002, .shift = 53},
    {.mask = 0x8080808080876, .magic = 0x80080280841000, .shift = 53},
    {.mask = 0x1010101010106e, .magic = 0x4200042010460008, .shift = 53},
    {.mask = 0x2020202020205e, .magic = 0x4800a0003040080, .shift = 53},
    {.mask = 0x4040404040403e, .magic = 0x400110082041008, .shift = 53},
    {.mask = 0x8080808080807e, .magic = 0x8000a041000880, .shift = 52},
    {.mask = 0x1010101017e00, .magic = 0x10138001a080c010, .shift = 53},
    {.mask = 0x2020202027c00, .magic = 0x804008200480, .shift = 54},
    {.mask = 0x4040404047a00, .magic = 0x10011012000c0, .shift = 54},
    {.mask = 0x8080808087600, .magic = 0x22004128102200, .shift = 54},
    {.mask = 0x10101010106e00, .magic = 0x200081201200c, .shift = 54},
    {.mask = 0x20202020205e00, .magic = 0x202a001048460004, .shift = 54},
    {.mask = 0x40404040403e00, .magic = 0x81000100420004, .shift = 54},
    {.mask = 0x80808080807e00, .magic = 0x4000800380004500, .shift = 53},
    {.mask = 0x10101017e0100, .magic = 0x208002904001, .shift = 53},
    {.mask = 0x20202027c0200, .magic = 0x90004040026008, .shift = 54},
    {.mask = 0x40404047a0400, .magic = 0x208808010002001, .shift = 54},
    {.mask = 0x8080808760800, .magic = 0x2002020020704940, .shift = 54},
    {.mask = 0x101010106e1000, .magic = 0x8048010008110005, .shift = 54},
    {.mask = 0x202020205e2000, .magic = 0x6820808004002200, .shift = 54},
    {.mask = 0x404040403e4000, .magic = 0xa80040008023011, .shift = 54},
    {.mask = 0x808080807e8000, .magic = 0xb1460000811044, .shift = 53},
    {.mask = 0x101017e010100, .magic = 0x4204400080008ea0, .shift = 53},
    {.mask = 0x202027c020200, .magic = 0xb002400180200184, .shift = 54},
    {.mask = 0x404047a040400, .magic = 0x2020200080100380, .shift = 54},
    {.mask = 0x8080876080800, .magic = 0x10080080100080, .shift = 54},
    {.mask = 0x1010106e101000, .magic = 0x2204080080800400, .shift = 54},
    {.mask = 0x2020205e202000, .magic = 0xa40080360080, .shift = 54},
    {.mask = 0x4040403e404000, .magic = 0x2040604002810b1, .shift = 54},
    {.mask = 0x8080807e808000, .magic = 0x8c218600004104, .shift = 53},
    {.mask = 0x1017e01010100, .magic = 0x8180004000402000, .shift = 53},
    {.mask = 0x2027c02020200, .magic = 0x488c402000401001, .shift = 54},
    {.mask = 0x4047a04040400, .magic = 0x4018a00080801004, .shift = 54},
    {.mask = 0x8087608080800, .magic = 0x1230002105001008, .shift = 54},
    {.mask = 0x10106e10101000, .magic = 0x8904800800800400, .shift = 54},
    {.mask = 0x20205e20202000, .magic = 0x42000c42003810, .shift = 54},
    {.mask = 0x40403e40404000, .magic = 0x8408110400b012, .shift = 54},
    {.mask = 0x80807e80808000, .magic = 0x18086182000401, .shift = 53},
    {.mask = 0x17e0101010100, .magic = 0x2240088020c28000, .shift = 53},
    {.mask = 0x27c0202020200, .magic = 0x1001201040c004, .shift = 54},
    {.mask = 0x47a0404040400, .magic = 0xa02008010420020, .shift = 54},
    {.mask = 0x8760808080800, .magic = 0x10003009010060, .shift = 54},
    {.mask = 0x106e1010101000, .magic = 0x4008008008014, .shift = 54},
    {.mask = 0x205e2020202000, .magic = 0x80020004008080, .shift = 54},
    {.mask = 0x403e4040404000, .magic = 0x282020001008080, .shift = 54},
    {.mask = 0x807e8080808000, .magic = 0x50000181204a0004, .shift = 53},
    {.mask = 0x7e010101010100, .magic = 0x48fffe99fecfaa00, .shift = 53},
    {.mask = 0x7c020202020200, .magic = 0x48fffe99fecfaa00, .shift = 54},
    {.mask = 0x7a040404040400, .magic = 0x497fffadff9c2e00, .shift = 54},
    {.mask = 0x76080808080800, .magic = 0x613fffddffce9200, .shift = 54},
    {.mask = 0x6e101010101000, .magic = 0xffffffe9ffe7ce00, .shift = 54},
    {.mask = 0x5e202020202000, .magic = 0xfffffff5fff3e600, .shift = 54},
    {.mask = 0x3e404040404000, .magic = 0x10301802830400, .shift = 54},
    {.mask = 0x7e808080808000, .magic = 0x510ffff5f63c96a0, .shift = 53},
    {.mask = 0x7e01010101010100, .magic = 0xebffffb9ff9fc526, .shift = 52},
    {.mask = 0x7c02020202020200, .magic = 0x61fffeddfeedaeae, .shift = 53},
    {.mask = 0x7a04040404040400, .magic = 0x53bfffedffdeb1a2, .shift = 53},
    {.mask = 0x7608080808080800, .magic = 0x127fffb9ffdfb5f6, .shift = 53},
    {.mask = 0x6e10101010101000, .magic = 0x411fffddffdbf4d6, .shift = 53},
    {.mask = 0x5e20202020202000, .magic = 0x801000804000603, .shift = 53},
    {.mask = 0x3e40404040404000, .magic = 0x3ffef27eebe74, .shift = 53},
    {.mask = 0x7e80808080808000, .magic = 0x7645fffecbfea79e, .shift = 52},
}};

} // namespace fgfwr::move
