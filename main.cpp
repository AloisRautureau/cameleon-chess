#include <iostream>
#include "src/position.h"

int main() {
    using namespace Chameleon;

    Position pos;
    std::cout << pos << std::endl;

    pos.setFEN("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2 ");

    std::cout << pos << std::endl;
    return 0;
}
