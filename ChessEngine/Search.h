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
    int depth;

public:
    Search(Board& board, Evaluation eval, int depth): board(board), depth(depth), eval(eval){}

    /*
     * Search the best move given the current position (root node)
     */
    MOVEBITS bestMove(){
        int bestScore = -999999;
        MOVEBITS bestMove;

        int moveStackIndex;
        MOVEBITS moveStack[256];

        board.gen();
        //Copy the generated moves
        moveStackIndex = board.moveListIndx;
        for(int i = 0; i < moveStackIndex; i++){
            moveStack[i] = board.moveList[i];
        }
        bestMove = moveStack[0];

        //We give a score to each move using alpha-beta search
        for(int move = 0; move < moveStackIndex; move++){
            if(board.makeMove(moveStack[move])){
                int score = searchNode(-99999, 99999, depth - 1);
                board.unmake();

                if(score > bestScore){
                    bestMove = moveStack[move];
                    bestScore = score;
                }
            }
        }
        std::cout << bestScore << std::endl;
        return bestMove;
    }

    /*
     * Called to recursively search a given node if there is still depth left, else it just call quiescence and
     * returns a score. It uses a negamax algorithm
     */
    int searchNode(int alpha, int beta, int depthleft){
        if(depthleft==0){
            return quiescence(alpha, beta);
        }

        //Generate and copy moves
        int moveStackIndex;
        MOVEBITS moveStack[256];

        board.gen();
        //Copy the generated moves
        moveStackIndex = board.moveListIndx;
        for(int i = 0; i < moveStackIndex; i++){
            moveStack[i] = board.moveList[i];
        }

        //For each move, call searchNode recusively
        for(int move = 0; move < moveStackIndex; move++){
            if(board.makeMove(moveStack[move])){
                int score = -searchNode(-beta, -alpha, depthleft-1);
                board.unmake();

                if(score >= beta) return beta;
                if(score > alpha) alpha = score;
            }
        }
        return alpha;
    }

    int quiescence(int alpha, int beta){
        int standPat = eval.evaluation();
        if(standPat >= beta) return beta;
        if(standPat > alpha) alpha = standPat;

        //Generate and copy moves
        int moveStackIndex;
        MOVEBITS moveStack[256];

        board.genCaps();
        //If no captures are available, return alpha
        if(board.moveListIndx == 0) return alpha;
        //Copy the generated moves
        moveStackIndex = board.moveListIndx;
        for(int i = 0; i < moveStackIndex; i++){
            moveStack[i] = board.moveList[i];
        }

        //Check every capture
        for(int cap = 0; cap < moveStackIndex; cap++){
            if(board.makeMove(cap)){
                int score = -quiescence(-beta, -alpha);
                board.unmake();

                if(score >= beta) return beta;
                if(score > alpha) alpha = score;
            }
        }

        return alpha;
    }
};

#endif //BAUB_CHESS_SEARCH_H
