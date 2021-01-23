#include <iostream>
#include "ChessEngine/Board.h"


int main() {
    Board board;
    //board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");

    //board.makeMove(33280);
    //board.gen();

    board.perftTest();
    return 0;
}
