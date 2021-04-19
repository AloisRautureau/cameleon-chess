//
// Created by bitterboyy on 4/19/21.
//

#include <iostream>
#include "search.h"

using namespace Chameleon;

namespace Search {
    movebyte search_root(position &pos, int maxdepth, int movetime){
        movebyte best;
        int score = -99999;
        int best_score = -99999;

        int alpha = -99999;
        int beta = 99999;
        int aspiration_window = 50;

        //We first need to generate every move for the root position, then check every single one of them to find our best move
        movestack stack;
        pos.gen(stack);

        //The actual search
        int depth = 1;
        for(depth; depth < maxdepth; depth++) {
            for(int i = 0; i < stack.size; i++){
                pos.make(stack.moves[i]);
                score = -search_node(pos, depth - 1, -beta, -alpha);
                if(score <= alpha || score >= beta) { //Case where we evaluate outside of the window
                    alpha = -99999;
                    beta = 99999;
                    score = -search_node(pos, depth - 1, -beta, -alpha);
                }
                pos.unmake();

                if(score > best_score) {
                    best_score = score;
                    best = stack.moves[i];
                }
            }
            //After searching every move we adjust the window
            alpha = best_score + aspiration_window;
            beta = best_score - aspiration_window;
            //And send some info
            std::cout << "info "
            << "depth " << depth
            << " score cp " << best_score
            << std::endl;
        }

        //We searched everything, now we can just return the best move found
        return best;
    }

    int search_node(position &pos, int depth, int alpha, int beta) {
        //When reaching the max depth, return eval (TODO : return quiescence search result)
        if(depth == 0) return Eval::eval(pos);

        movestack stack;
        pos.gen(stack);

        int score;
        for(int i = 0; i < stack.size; i++) {
            pos.make(stack.moves[i]);
            score = -search_node(pos, depth - 1, -beta, -alpha);
            pos.unmake();

            if(score >= beta) return score;
            if(score > alpha) alpha = score;
        }
        return alpha;
    }
}