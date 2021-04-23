#include <iostream>
#include "src/uci.h"

using namespace Chameleon;

int main() {
    inithash(); //Always call that first, otherwise AI goes boom

    UCI::listen();

    return 0;
}
