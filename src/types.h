#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

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

// Converts a given sqare index to its string representation
inline constexpr std::string_view SQ_STRING[] {
  "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
  "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
  "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
  "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
  "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
  "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
  "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
  "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
  "-"
};

enum color_t {
  BLACK, WHITE, NONE
};

enum piece_t {
  PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY
};

// Representation for pieces
inline constexpr char PIECE_STR_REP[2][6] {
    {'p', 'n', 'b', 'r', 'q', 'k'},
    {'P', 'N', 'B', 'R', 'Q', 'K'},
};

// Moves are represented over 16 bits
// 6 bits from, 6 bits to, 4 bits flag
using move_t = unsigned int;

// Move flags
enum mvflag_t {
  QUIET, DPAWNPUSH, KCASTLE, QCASTLE, CAP,
  NPROM = 8, BPROM, RPROM, QPROM, NPROMCAP, BPROMCAP, RPROMCAP, QPROMCAP
};

inline move_t encode(sq_t from, sq_t to, mvflag_t flag) {
  return ((from << 10) + (to << 4) + flag);
}
inline sq_t from(const move_t& move) {
  return sq_t(move >> 10);
}
inline sq_t to(const move_t& move) {
  return sq_t((move&0x03F0) >> 4);
}
inline mvflag_t flag(const move_t& move) {
  return mvflag_t(move&0xF);
}


// Castling rights are represented over 4 bits
inline constexpr int WKCASTLE { 0b1000 };
inline constexpr int WQCASTLE { 0b0100 };
inline constexpr int BKCASTLE { 0b0010 };
inline constexpr int BQCASTLE { 0b0001 };

#endif