#include <iostream>
#include "src/position.h"
#include "src/eval.h"
#include "src/search.h"

using namespace Chameleon;

int main() {
    system("clear");

    position test = position();
    movestack stack;
    test.show();
    movebyte move = Search::search_root(test, 7);
    std::cout << intToSq(fromSq(move)) << intToSq(toSq(move)) << std::endl;

    return 0;
}
