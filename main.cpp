#include <iostream>
#include "src/position.h"

using namespace Chameleon;

int main() {
    system("clear");

    position test = position();
    movestack stack;
    test.gen(stack);

    return 0;
}
