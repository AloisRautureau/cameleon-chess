#ifndef POSITION_H
#define POSITION_H

#include "types.h"
#include "bitboard.h"

namespace Chameleon {

// Representation for pieces
inline constexpr char PIECE_STR_REP[2][6] {
    {'P', 'N', 'B', 'R', 'Q', 'K'},
    {'p', 'n', 'b', 'r', 'q', 'k'}
};

class Position {
private:
    color_t m_side { WHITE }; // Side to move
    bitboard_t m_ptype_bb[6] { // Bitboard by piece_type
        0x00FF00000000FF00, 0x4200000000000042, 0x2400000000000024,
        0x8100000000000081, 0x800000000000008, 0x1000000000000010,
    };
    bitboard_t m_color_bb[2] { // Bitboard by piece color
        0x000000000000FFFF,
        0xFFFF000000000000
    };
    bitboard_t m_empty_bb { 0x0000FFFFFFFF0000 }; // Empty squares bitboard

    piece_t m_board8x8[64] { // Keeps track of which piece sits where 
        ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
        PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
        ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
    };

    int m_ply { 0 }; // Ply from game start
    int m_halfmove { 0 }; // Halfmove clock

    sq_t m_ep { inv }; // En passant square

public:
    void make(move_t move); // Makes the given move on the board
    void unmake(); // Takes back the las move made

    void setFEN(std::string fen); // Sets pos to given FEN string
    std::string getFEN(); // Returns the FEN string of the current pos

    // Color of the piece sitting on given square
    color_t color(sq_t sq) const;


    friend std::ostream& operator<< (std::ostream &out, const Position &pos);
};



}

#endif