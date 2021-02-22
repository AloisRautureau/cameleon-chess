//
// Created by bitterboyy on 2/22/21.
//

#include "debug.h"

unsigned long long debug::perftRecursive(int depth, board_representation board) {
    movebits stack[256];
    int stackIndex = 0;

    board.gen();
    for(movebits move : board.m_moveStack){
        stack[stackIndex] = move;
        stackIndex++;
    }
}
