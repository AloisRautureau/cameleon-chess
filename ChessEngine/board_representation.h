//
// Created by bitterboyy on 2/20/21.
//

#ifndef BAUB_CHESS_BOARD_REPRESENTATION_H
#define BAUB_CHESS_BOARD_REPRESENTATION_H

#include "bb.h"
#include <vector>

/*
 * This class is the representation of a board state.
 * It should be capable of :
 * - generating every legal move in a current position
 * - make and unmake moves
 * - tell if a position is checkmate or stalemate
 */

class board_representation {
private:
    /*
     * We use a 2D array of 2x6, or one array per side with one bitboard per piece type
     * The boards use Little-Endian Rank-File mapping (LERF for short), a fancy way of saying a1 = 0, b1 = 1... and h8 = 63
     */

    bitboard m_pieces[2][6];
    enum enumSquare {
        a1, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8
    };

    //Variables used to keep track of the game state
    bool m_sideToMove;
    char m_castlingRights;

};


#endif //BAUB_CHESS_BOARD_REPRESENTATION_H
