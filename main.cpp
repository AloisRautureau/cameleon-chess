#include <iostream>
#include "ChessEngine/Board.h"
#include "ChessEngine/Search.h"


int main() {
    Board board;
    Search search(board, Evaluation(board), 4);
    //board.setFEN("4k3/8/8/2q1b3/3P4/8/8/4K3 w - - 0 1");

    /*board.showBoard();
    while(true){
        board.makeMove(search.bestMove());
        board.showBoard();
        //std::cin.get();
    }
     */



    board.perftTest();

    /*
     *  bugs to fix
     *
     *  TODO Problem when updating piecelist (pieces appear for some reason)
     */

    return 0;
}
