//
// Created by bitterboyy on 2/22/21.
//

#ifndef BAUB_CHESS_DEBUG_H
#define BAUB_CHESS_DEBUG_H

#include "board_representation.h"

class debug {
public:
    unsigned long long perftRecursive(int depth, board_representation board);
    void perft(const board_representation &board);
};


#endif //BAUB_CHESS_DEBUG_H
