#ifndef TYPES_H
#define TYPES_H

#include <string>

using bitboard_t = unsigned long long;

// Square enumeration for LERF mapping of bitboards
enum sq_t {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8,
  inv
};

enum color_t {
    WHITE, BLACK
};

enum piece_t {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY
};

// Moves are represented over 16 bits
// 6 bits from, 6 bits to, 4 bits flag
using move_t = unsigned int;

#endif