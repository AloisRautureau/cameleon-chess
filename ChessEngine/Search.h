//
// Created by bitterboyy on 1/26/21.
//

#ifndef BAUB_CHESS_SEARCH_H
#define BAUB_CHESS_SEARCH_H

#include "Evaluation.h"

/*
 * The Search class will, given a board and a depth, serach the best move from the board's position using alpha-beta pruning
 */

class Search{
private:
    /*
     * The search functions need to make changes to the board, as well as evaluate.
     * Therefore, we need boath to be implemented
     */
    Board& board;
    Evaluation eval;
    const int depth;

public:
    Search(Board& board, Evaluation eval, int depth): board(board), depth(depth), eval(eval){}
};

#endif //BAUB_CHESS_SEARCH_H
