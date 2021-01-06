//
// Created by bitterboyy on 12/29/20.
//
#include "Engine.h"
#include <random>

Engine::Engine(int depth, BoardRepresentation* board) : m_board(board), m_depth(depth){}

int Engine::quiescence(int alpha, int beta){
    int evalActuelle = m_board->evalutation();
    if(evalActuelle >= beta) return beta;
    if(alpha < evalActuelle) alpha = evalActuelle;

    //On choppe les captures
    m_board->generateCaptures();
    std::vector<std::vector<int>> captures = m_board->getAttackingMoves();

    //We check every capture
    if(!captures.empty()){
        for(std::vector<int> capture : captures){
            m_board->makeMove(capture.at(0), capture.at(1), capture.at(2));
            int eval = -quiescence(-beta, -alpha);
            m_board->takeback();

            int delta = 1000;

            if(eval >= beta) return beta;
            if(eval < alpha - delta) return alpha;
            if(eval > alpha) alpha = eval;
        }
    }
    else return evalActuelle;

    return alpha;
};

int Engine::alphaBeta(int alpha, int beta, int depth){
    if(depth == 0){
        return quiescence(alpha, beta);
    }

    //Genere les coups
    m_board->moveGenerator();
    std::vector<std::vector<int>> moves = m_board->getMoves();

    if(m_board->checkmated() == 1){
        return m_board->getSide() == 1 ? -10000 : 10000;
    }

    if(moves.empty()) return 0;

    //For each move, we call alphaBeta recursively and invert alpha and beta
    for(std::vector<int> move : moves){
        m_board->makeMove(move.at(0), move.at(1), move.at(2));
        int eval = -alphaBeta(-beta, -alpha, depth - 1);
        m_board->takeback();
        if(eval >= beta)
            return beta;
        if(eval > alpha) {
            alpha = eval;
        }

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
        std::cout << "Testing " << BoardRepresentation::adressToMoveParser(move.at(0)) << " -> " << BoardRepresentation::adressToMoveParser(move.at(1)) << std::endl;
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