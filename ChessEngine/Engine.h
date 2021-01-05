//
// Created by bitterboyy on 12/29/20.
//

#ifndef BAUB_CHESS_ENGINE_H
#define BAUB_CHESS_ENGINE_H

#include "BoardRepresentation.h"


class Engine{
private:
    int m_depth;
    BoardRepresentation* m_board;
    std::stack<std::vector<int>> m_pvnodes;

public:
    explicit Engine(int depth, BoardRepresentation* board);
    int quiescence(int alpha, int beta);
    int alphaBeta(int alpha, int beta, int depth);
    std::vector<int> search();
};


#endif //BAUB_CHESS_ENGINE_H
