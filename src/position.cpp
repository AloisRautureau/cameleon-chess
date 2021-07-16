#include "position.h"

namespace Chameleon {

std::ostream& operator<< (std::ostream &out, const Position &pos) {
    std::string board {
        " - - - - - - - - - - - -\n"
        " - - - - - - - - - - - -\n"
        " 8 - . . . . . . . . - -\n"
        " 7 - . . . . . . . . - -\n"
        " 6 - . . . . . . . . - -\n"
        " 5 - . . . . . . . . - -\n"
        " 4 - . . . . . . . . - -\n"
        " 3 - . . . . . . . . - -\n"
        " 2 - . . . . . . . . - -\n"
        " 1 - . . . . . . . . - -\n"
        " - - - - - - - - - - - -\n"
        " - - A B C D E F G H - -\n"
    };

    // For each bitboard, replace the given square with the piece
    // representation
    int str_indx  { 55 };
    sq_t sq       { };
    for(int rank { 7 }; rank >= 0; --rank) {
        for(int file { 0 }; file < 8; ++file) {
            sq = sq_t(rank*8 + file);
            if(pos.m_board8x8[sq] != EMPTY) {
                board[str_indx] = PIECE_STR_REP[pos.color(sq)][pos.m_board8x8[sq]];
            }
            str_indx += 2;
        }
        str_indx += 9;
    }

    std::string info { "\n side: " };
    info += pos.m_side ? "black" : "white";
    info += "\tply: ";
    info += std::to_string(pos.m_ply); 

    // Then we finally output that to the stream
    out << board;
    out << info;

    return out;
}

color_t Position::color(sq_t sq) const {
    return (ONEBB << sq) & m_color_bb[WHITE] ? WHITE : BLACK;
}

}