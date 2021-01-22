#include <iostream>
#include "ChessEngine/Board.h"


int main() {
    Board board;
    //board.makeMove(33280);
    //board.gen();

    board.perftTest();
    return 0;
}
