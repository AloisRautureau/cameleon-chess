//
// Created by bitterboyy on 2/20/21.
//

#ifndef BAUB_CHESS_BB_H
#define BAUB_CHESS_BB_H

#include <iostream>

//This class will contain every bitboard method

typedef unsigned long long int bitboard;

class bb {
public:
    static const bitboard EMPTY = 0;
    static const bitboard UNIVERSE = 0xffffffffffffffff;

    /**
     *
     * @param bb any bitboard
     * @return whether or not bb is single populated
     */
    static bool isSinglePop(bitboard bb);

    //This is an algorithm mentioned by Brian Kernighan in "The C Programming_Language", 2nd Edition 1988
    //it basically resets the LS1B (Least Significant set bit) until the bitboard is empty, and returns a counter.
    static int popCount(bitboard bb);

    //Those next functions are there to rotate/flip bitboards

    static bitboard verticalFlip(bitboard bb);

    static bitboard horizontalFlip(bitboard bb);

    static bitboard diagonalFlip(bitboard bb);

    static bitboard antiDiagonalFlip(bitboard bb);

     static bitboard rotate180(bitboard bb);

    static bitboard rotate90clock(bitboard bb);

    static bitboard rotate90anti(bitboard bb);

    //prints out the bitboard with . = 0
    static void print(bitboard bb);


};


#endif //BAUB_CHESS_BB_H
