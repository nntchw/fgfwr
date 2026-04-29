#include "fci/fen.hh"

#include "types/castling_rights.hh"
#include "types/color.hh"
#include "types/piece.hh"
#include "types/position.hh"
#include "types/square.hh"

#include <cctype>
#include <string_view>

using namespace fgfwr::types;

auto fgfwr::fci::parse_fen(std::string_view fen) -> Position {
    Position pos;

    int i = 0;

    // 1. Piece placement
    Square sq = 0;
    while (fen[i] != ' ') {
        char c = fen[i++];

        // We can ignore '/' since our board is already indexed top-down.
        if (c == '/')
            continue;

        if (std::isdigit(c)) {
            sq += c - '0';
            continue;
        }

        const Color color = (std::isupper(c)) ? Color::White : Color::Black;
        const Piece piece = [&] -> auto {
            switch (std::tolower(c)) {
            case 'p':
                return Piece::Pawn;
            case 'n':
                return Piece::Knight;
            case 'b':
                return Piece::Bishop;
            case 'r':
                return Piece::Rook;
            case 'q':
                return Piece::Queen;
            case 'k':
                return Piece::King;
            default:
                return Piece::Null;
            }
        }();

        pos.add_piece(color, piece, sq++);
    }

    ++i; // skip space

    // 2. Side to move
    pos.set_stm(fen[i] == 'w' ? Color::White : Color::Black);
    i += 2; // "w " or "b "

    // 3. Castling rights
    CastlingRights rights{};
    if (fen[i] == '-') {
        ++i;
    } else {
        while (fen[i] != ' ') {
            switch (fen[i]) {
            case 'K':
                rights.set_mask(CastlingMask::WhiteShort);
                break;
            case 'Q':
                rights.set_mask(CastlingMask::WhiteLong);
                break;
            case 'k':
                rights.set_mask(CastlingMask::BlackShort);
                break;
            case 'q':
                rights.set_mask(CastlingMask::BlackLong);
                break;
            }
            ++i;
        }
    }

    pos.set_cr(rights);
    ++i; // skip space

    // 4. En passant
    if (fen[i] == '-') {
        pos.set_ep_square(Square::Null);
        ++i;
    } else {
        int file = fen[i++] - 'a';
        int rank = fen[i++] - '1';
        pos.set_ep_square(8 * (7 - rank) + file);
    }

    ++i; // skip space

    // 5. Halfmove clock
    int hmc = 0;
    while (i < fen.size() and std::isdigit(fen[i])) {
        hmc = hmc * 10 + (fen[i++] - '0');
    }
    pos.set_hmc(hmc);

    return pos;
}
