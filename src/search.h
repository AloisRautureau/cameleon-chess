//
// Created by bitterboyy on 4/19/21.
//

#ifndef CHAMELEON_SEARCH_H
#define CHAMELEON_SEARCH_H

#include "defs.h"
#include "eval.h"

using namespace Chameleon;

namespace Search {
    //This is the base function, which searches the root nodes (every move from the given position) and outputs a
    //best move.
    //This is where we implement most tricks to reduce search space, like iterative deepening, aspiration windows, etc etc
    movebyte search_root(position &pos, int maxdepth = 99, int movetime = 99999);

    //This is a recursive function that searches a node by searching every child node up until a certain depth or we run out of time
    int search_node(position &pos, int depth, int alpha, int beta);

    //Helps us reduce the horizon effect by searching every noisy move until we reach a quiet enough position to not blunder
    int quiescence(position &pos, int alpha, int beta);
}


#endif //CHAMELEON_SEARCH_H
