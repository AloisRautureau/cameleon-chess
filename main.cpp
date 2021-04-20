#include <iostream>
#include "src/position.h"
#include "src/eval.h"
#include "src/search.h"

using namespace Chameleon;

int main() {
    system("clear");

    position test = position();
    movestack stack;
    test.setFEN("1r2r1k1/3bnppp/p2q4/2RPp3/4P3/6P1/2Q1NPBP/2R3K1 w - - 0 1");
    test.show();
    movebyte move = Search::search_root(test, 7);
    std::cout << intToSq(fromSq(move)) << intToSq(toSq(move)) << std::endl;
    return 0;
}
