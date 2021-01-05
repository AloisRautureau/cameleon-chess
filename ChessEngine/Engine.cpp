//
// Created by bitterboyy on 12/29/20.
//
#include "Engine.h"
#include <random>

Engine::Engine(int depth, BoardRepresentation* board) : m_board(board), m_depth(depth){}

//The idea for minimax algorithm is that :
/*
 * For each possible move, the algorithm must check each possible move, until desired depth is reached.
 * Then, it must throw the result (board.evaluation()) back at the main function in order to add/decrease the strenght of that move
 * All of that, while not modifying the stat of the board in the end
*/

int Engine::alphaBeta(int alpha, int beta, int depth){
    if(depth == 0) return m_board->evalutation();

    if(m_board->checkmated() == 1){
        return m_board->getSide() == 1 ? -10000 : 10000;
    }

    if(m_board->checkmated() == -1){
        return 0;
    }

    //Genere les coups
    m_board->moveGenerator();
    std::vector<std::vector<int>> moves = m_board->getMoves();

    //For each move, we call alphaBeta recursively and invert alpha and beta
    for(std::vector<int> move : moves){
        m_board->makeMove(move.at(0), move.at(1), move.at(2));
        int eval = alphaBeta(-beta, -alpha, depth - 1);
        m_board->takeback();
        if(eval >= beta)
            return beta;
        if(eval > alpha)
            alpha = eval;
    }

    return alpha;
}

std::vector<int> Engine::search() {

    //Stocke les valeurs de chaque coup root+1
    std::vector<int> bestMove;
    int bestValue = -99999;
    std::vector<int> worstMove;
    int worstValue = 99999;

    //On génère les coups root+1
    m_board->moveGenerator();
    std::vector<std::vector<int>> moves = m_board->getMoves();

    for(std::vector<int> move : moves){
        //std::cout << "Testing " << BoardRepresentation::adressToMoveParser(move.at(0)) << " -> " << BoardRepresentation::adressToMoveParser(move.at(1)) << std::endl;
        //Pour chaque coup, on appelle alphabeta pour trouver la valeur de ce coup
        m_board->makeMove(move.at(0), move.at(1), move.at(2));
        int score = alphaBeta(-99999, 99999, m_depth);
        m_board->takeback();
        //std::cout << score << std::endl;
        if(score > bestValue){
            bestValue = score;
            bestMove.clear();
            bestMove.push_back(move.at(0));
            bestMove.push_back(move.at(1));
            bestMove.push_back(move.at(2));
        }
        
        if(score < worstValue){
            worstValue = score;
            worstMove.clear();
            worstMove.push_back(move.at(0));
            worstMove.push_back(move.at(1));
            worstMove.push_back(move.at(2));
        }
    }

    std::cout << std::endl << "Best move : " << BoardRepresentation::adressToMoveParser(bestMove.at(0)) << " -> " << BoardRepresentation::adressToMoveParser(bestMove.at(1)) << " " << bestValue <<std::endl;
    std::cout << "Worst move : " << BoardRepresentation::adressToMoveParser(worstMove.at(0)) << " -> " << BoardRepresentation::adressToMoveParser(worstMove.at(1)) << " " << worstValue << std::endl << std::endl;
    return m_board->getSide() == 1 ? bestMove : worstMove;
}