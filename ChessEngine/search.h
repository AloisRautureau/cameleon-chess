//
// Created by bitterboyy on 3/13/21.
//

#ifndef CHAMELEON_SEARCH_H
#define CHAMELEON_SEARCH_H

#include "position.h"
#include <chrono>
#include "evaluation.h"
#include "display.h"

namespace Chameleon{
    namespace Search{
        //Searches the root node of a position with a few options:
        //- maxdepth sets a maximum depth after which the search must not go any further
        //- maxTime sets a maximum search time, after which the search stops and returns the best move found
        //- moveList sets a given subset of moves that should be searched, ignoring any other moves that may have been generated
        //- maxNodes sets a number of nodes after which the search must stop
        //- infinite is a flag that, when set, make the search ignore any of the above set values
        movebits bestMove(position &position, int maxdepth = 0, int maxTime = 0, const std::vector<movebits> &moveList = {}, long long maxNodes = 0, bool infinite = false);

        //Searches nodes recursively, called from bestMove. It returns a score
        int searchNode(position &position, int alpha, int beta, int depthLeft, long long &maxNodes, int maxTime);

        //Quiescence ensures that we end up in a position where we're not gonna lose our queen next move,
        //it helps reduce the horizon effect
        int quiescence(position &position, int alpha, int beta, int maxTime);

        //This function returns the time to allocate to a search given the time left and increment for side to move
        int manageTime(position &position);


        //### TRANSPOSITION TABLE ###
        //A transposition table is used to keep info about notable positions in memory
        //This way, if we end up searching said position again, we already know the best move, saving the search!
        class ttable{

        };
    }
}

#endif //CHAMELEON_SEARCH_H
