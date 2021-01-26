#include <iostream>
#include "ChessEngine/Board.h"
#include "ChessEngine/Search.h"


int main() {
    Board board;
    Search search(board, Evaluation(board), 5);
    //board.setFEN("4k3/8/8/2q1b3/3P4/8/8/4K3 w - - 0 1");

    //board.perftTest();

    /*
     *  bugs to fix
     *
     *  TODO Problem when updating piecelist (most likely caused by captures since it appeared with quiescence)
     */

    return 0;
}
