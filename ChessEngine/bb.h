//
// Created by bitterboyy on 2/20/21.
//

#ifndef BAUB_CHESS_BB_H
#define BAUB_CHESS_BB_H

//This class will contain every bitboard method

typedef unsigned long long int bitboard;

class bb {
    static const bitboard EMPTY = 0;
    static const bitboard UNIVERSE = 0xffffffffffffffff;

    static bool isSinglePop(bitboard bb){
        return bb && (bb & (bb-1)) == 0;
    }

    //Returns the number of set bits in a given bitboard
    //This is an algorithm mentioned by Brian Kernighan in "The C Programming_Language", 2nd Edition 1988
    //it basically resets the LS1B (Least Significant set bit) until the bitboard is empty, and returns a counter.
    static int popCount(bitboard bb){
        int counter = 0;
        while(bb){
            counter++;
            bb &= bb - 1;
        }
        return counter;
    }
};


#endif //BAUB_CHESS_BB_H
