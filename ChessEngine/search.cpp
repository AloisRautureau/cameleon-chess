//
// Created by bitterboyy on 3/13/21.
//

#include "search.h"

namespace Chameleon{
    namespace Search{
        movebits bestMove(position &position, int maxdepth, int maxTime, const std::vector<movebits>& moveList, unsigned long long maxNodes, bool infinite){
            //Variables keeping track of the best move found and its score
            movebits bestMove;
            movebits currentMove;
            int currentScore;
            int bestScore{-99999};

            //That part is used to implement the aspiration window
            //The idea is that after a first search, we consider that the results of the next one won't be too far off
            //So we can change the window for cutoffs, making more of them happen!
            int alpha = -99999;
            int beta = 99999;
            int aspirationWindow = 25;

            //Keep track of time spent on the search
            int timeSpent;
            std::chrono::duration<int> durationObject{};

            //The nodes part
            unsigned long long nodesSearched;

            //Storing generated moves
            movebits mvStack[256];
            int mvStackIndx{0};
            if(moveList.empty()){ //If no move list was given, just generate all moves
                position.gen(mvStack, mvStackIndx);
            } else { //Otherwise, store them
                for(auto move : moveList){
                    mvStack[mvStackIndx++] = move;
                }
            }

            //If no max depth was given, we assume that the search is meant to not care about depth and set it to be infinite
            if(!maxdepth || infinite) maxdepth = INT32_MAX;
            //Same with nodes
            if(!maxNodes || infinite) maxNodes = 0xFFFFFFFFFFFFFFFF;

            //Keep track of the start time of the search
            auto start = std::chrono::high_resolution_clock::now();
            //We can start the actual search now, going through each move in the stack
            //then comparing them with the bestMove that was found unti we hit a stop condition
            //We will do that while incrementing the depth to go to (iterative deepening)
            for(int depth = 1; depth < maxdepth; depth++){
                for(int i = 0; i < mvStackIndx; i++){
                    currentMove = mvStack[i];
                    if(position.make(currentMove)){
                        maxNodes--;
                        std::cout << "info currmove " << display::displayMove(currentMove) << " currmovenumber " << i << std::endl;
                        currentScore = -searchNode(position, -beta, -alpha, depth - 1, maxNodes);

                        //The bad side of aspiration windows: to make sure we don't miss stuff, if the score is too far
                        //we need to recalculate, because our window isn't good
                        if(currentScore <= alpha || currentScore >= beta){
                            //Reset alpha and beta
                            alpha = -99999;
                            beta = 99999;
                            aspirationWindow *= 10;
                            currentScore = -searchNode(position, -beta, -alpha, depth - 1, maxNodes);
                        }
                        position.takeback();

                        //If the best score is less than what we got, we got ourselvs a new best move!
                        if(currentScore > bestScore){
                            bestScore = currentScore;
                            bestMove = currentMove;
                        }
                    }
                    nodesSearched = (0xFFFFFFFFFFFFFFFF-maxNodes);
                    timeSpent = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
                    if(maxTime && !infinite && timeSpent >= maxTime) break;
                    if(maxNodes && !infinite && nodesSearched >= maxNodes) break;
                }
                //Modify alpha and beta
                alpha = bestScore + aspirationWindow;
                beta = bestScore - aspirationWindow;

                nodesSearched = (0xFFFFFFFFFFFFFFFF-maxNodes);
                timeSpent = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
                std::cout << "info depth " << depth << " nodes " << 0xFFFFFFFFFFFFFFFF - maxNodes << " nps " << (int)(nodesSearched/(timeSpent*0.001)+1) << std::endl;
                if(maxTime && !infinite && timeSpent >= maxTime) break;
                if(maxNodes && !infinite && nodesSearched >= maxNodes) break;
            }
            return bestMove;
        }

        int searchNode(position &position, int alpha, int beta, int depthLeft, unsigned long long &maxNodes) {
            maxNodes--;
            //We just hit a stop condition
            if(!depthLeft || !maxNodes){
                //Call quiescence to reduce horizon effect
                return quiescence(position, alpha, beta);
            }

            //Generate and store moves
            movebits mvStack[256];
            int mvStackIndx{0};
            position.gen(mvStack, mvStackIndx);

            int copyBeta = beta;
            int score;
            for(int i = 0; i < mvStackIndx; i++){
                if(position.make(mvStack[i])){
                    score = -searchNode(position, -copyBeta, -alpha, depthLeft - 1, maxNodes);

                    //That's part of negascout implementation, it adds up to alpha-beta by using a window
                    //Basically, it increases the chances of a cutoff, but we need to re search if a score is out of the window
                    if(score > alpha && score < beta && i){
                        score = -searchNode(position, -beta, -alpha, depthLeft - 1, maxNodes);
                    }
                    position.takeback();

                    //Then it's just normal alpha beta stuff
                    if(alpha < score){
                        alpha = score;
                    }
                    if(alpha >= beta){
                        return alpha;
                    }

                    beta = alpha + 1; //We change the window for negascout
                }

            }
            return alpha;
        }

        int quiescence(position &position, int alpha, int beta) {
            int stand_pat = Evaluation::eval(position);
            if(stand_pat >= beta){
                return stand_pat;
            }
            if(stand_pat < alpha - 900){ //Delta pruning
                return alpha;
            }
            if(alpha < stand_pat){
                alpha = stand_pat;
            }

            int score;

            //Generate and store noisy moves only as those are the ones interesting to us
            movebits mvStack[256];
            int mvStackIndx{0};
            position.genNoisy(mvStack, mvStackIndx);

            for(int i = 0; i < mvStackIndx; i++){
                if(position.make(mvStack[i])){
                    score = -quiescence(position, -beta, -alpha);
                    position.takeback();

                    //Then it's just normal alpha beta stuff
                    if(alpha < score){
                        alpha = score;
                    }
                    if(score >= beta){
                        return beta;
                    }
                }
            }
            return alpha;
        }

    }
}