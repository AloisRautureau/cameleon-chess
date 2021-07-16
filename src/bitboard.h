#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"

namespace Chameleon {

// Bitboard constants
inline constexpr bitboard_t FILE_A  { 0x0101010101010101 };
inline constexpr bitboard_t FILE_H  { 0x8080808080808080 };
inline constexpr bitboard_t RANK_1  { 0x00000000000000FF };
inline constexpr bitboard_t RANK_8  { 0xFF00000000000000 };
inline constexpr bitboard_t FULLBB  { 0xFFFFFFFFFFFFFFFF };
inline constexpr bitboard_t EMPTYBB { 0x0000000000000000 };
inline constexpr bitboard_t ONEBB   { 0x0000000000000001 };

// Attacks by piece


// Bitboard functions
inline int bitscanForward(bitboard_t bb) {
    return __builtin_ffs(bb) - 1;
}; 
inline int bitscanReverse(bitboard_t bb){
    return 63 - __builtin_clz(bb);
};
    
}

#endif