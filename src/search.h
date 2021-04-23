//
// Created by bitterboyy on 4/19/21.
//

#ifndef CHAMELEON_SEARCH_H
#define CHAMELEON_SEARCH_H

#include "defs.h"
#include "eval.h"
#include <chrono>

using namespace Chameleon;

namespace Search {
    //Transposition table struct
    enum transpo_flag {
        EXACT,
        ALPHA,
        BETA,
    };
    struct ttable_entry {
        zhash hash{0};
        int depth{0};
        int score{0};
        int flag{0};
        movebyte move{0};
    };

    static const int R_FACTOR = 2;
    static uint64_t nodes_searched{0};
    static uint64_t quiescence_nodes{0};

    static const int table_size = 1000;
    static ttable_entry ttable[2][table_size]; //Stores information about positions that we've already encountered (0 -> replace by depth, 1 -> always replace)
    static movebyte killers[256]{0}; //Stores moves that caused a beta cutoff at a certain ply

    //This is the base function, which searches the root nodes (every move from the given position) and outputs a
    //best move.
    //This is where we implement most tricks to reduce search space, like iterative deepening, aspiration windows, etc etc
    movebyte search_root(position &pos, int maxdepth = 99, int movetime = 0, int timeleft = 99999999, int increment = 0, bool infinite = false);

    //This is a recursive function that searches a node by searching every child node up until a certain depth or we run out of time
    int search_node(position &pos, int depth, int alpha, int beta, bool null);

    //Helps us reduce the horizon effect by searching every noisy move until we reach a quiet enough position to not blunder
    int quiescence(position &pos, int alpha, int beta);

    /*
     * UTILITY FUNCTIONS
     */
    //Function used to calculate the mvoetime to give depending on time left and increment
    int timeboy(int timeleft, int increment);
}


#endif //CHAMELEON_SEARCH_H
