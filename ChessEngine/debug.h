//
// Created by bitterboyy on 2/22/21.
//

#ifndef BAUB_CHESS_DEBUG_H
#define BAUB_CHESS_DEBUG_H

#include "position.h"
#include "display.h"
#include <chrono>

namespace Chameleon{
    namespace Debug {
        void perft(const position &board);

        unsigned long long int
        perftRecursive(int depth, position board, int* caps, int* ep, int* castles, int* prom, int* check, int* mate);

        void perftDivide(position &board);

        void perftSuite(position &board, const std::vector<std::string>& fenStack);
    }
}

#endif //BAUB_CHESS_DEBUG_H
