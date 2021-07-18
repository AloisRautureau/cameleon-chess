#include <iostream>
#include "src/position.h"

int main() {
    using namespace Chameleon;

    Position pos;
    std::cout << pos << std::endl;

    pos.make(encode(e2, e4, DPAWNPUSH));

    std::cout << pos << std::endl;
    return 0;
}
