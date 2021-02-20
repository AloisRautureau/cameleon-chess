//
// Created by bitterboyy on 2/20/21.
//

#include "bb.h"

bool bb::isSinglePop(bitboard bb) {
    return bb && (bb & (bb-1)) == 0;
}

int bb::popCount(bitboard bb) {
    int counter = 0;
    while(bb){
        counter++;
        bb &= bb - 1;
    }
    return counter;
}

bitboard bb::verticalFlip(bitboard bb) {
    return  ( (bb << 56)                           ) |
            ( (bb << 40) & 0x00ff000000000000 ) |
            ( (bb << 24) & 0x0000ff0000000000 ) |
            ( (bb <<  8) & 0x000000ff00000000 ) |
            ( (bb >>  8) & 0x00000000ff000000 ) |
            ( (bb >> 24) & 0x0000000000ff0000 ) |
            ( (bb >> 40) & 0x000000000000ff00 ) |
            ( (bb >> 56) );
}

bitboard bb::horizontalFlip(bitboard bb) {
    const bitboard k1 = 0x5555555555555555;
    const bitboard k2 = 0x3333333333333333;
    const bitboard k4 = 0x0f0f0f0f0f0f0f0f;
    bb = ((bb >> 1) & k1) | ((bb & k1) << 1);
    bb = ((bb >> 2) & k2) | ((bb & k2) << 2);
    bb = ((bb >> 4) & k4) | ((bb & k4) << 4);
    return bb;
}

bitboard bb::diagonalFlip(bitboard bb) {
    bitboard t;
    const bitboard k1 = 0x5500550055005500;
    const bitboard k2 = 0x3333000033330000;
    const bitboard k4 = 0x0f0f0f0f00000000;
    t  = k4 & (bb ^ (bb << 28));
    bb ^=       t ^ (t >> 28) ;
    t  = k2 & (bb ^ (bb << 14));
    bb ^=       t ^ (t >> 14) ;
    t  = k1 & (bb ^ (bb <<  7));
    bb ^=       t ^ (t >>  7) ;
    return bb;
}

bitboard bb::antiDiagonalFlip(bitboard bb) {
    bitboard t;
    const bitboard k1 = 0xaa00aa00aa00aa00;
    const bitboard k2 = 0xcccc0000cccc0000;
    const bitboard k4 = 0xf0f0f0f00f0f0f0f;
    t  =       bb ^ (bb << 36) ;
    bb ^= k4 & (t ^ (bb >> 36));
    t  = k2 & (bb ^ (bb << 18));
    bb ^=       t ^ (t >> 18) ;
    t  = k1 & (bb ^ (bb <<  9));
    bb ^=       t ^ (t >>  9) ;
    return bb;
}

bitboard bb::rotate180(bitboard bb) {
    return(verticalFlip(horizontalFlip(bb)));
}

bitboard bb::rotate90clock(bitboard bb) {
    return(diagonalFlip(horizontalFlip(bb)));
}

bitboard bb::rotate90anti(bitboard bb) {
    return(antiDiagonalFlip(horizontalFlip(bb)));
}

void bb::print(bitboard bb) {
    for(int i = 0; i < 64; i++){
        if(!(i%8)) std::cout << std::endl;
        std::cout << ((bb & (0x01 << i)) ? '1' : '0');
    }
}




