#include <iostream>
#include "ChessEngine/position.h"
#include "ChessEngine/display.h"
#include "ChessEngine/debug.h"
#include "ChessEngine/evaluation.h"
#include "ChessEngine/search.h"

using namespace Chameleon;

int main() {
    system("clear");

    position pos;

    display::showPosition(pos);
    Evaluation::init();

    movebits best = Search::bestMove(pos, 18, 10000, {}, 0, false);

    std::cout << display::displayMove(best) << std::endl;

    return 0;
}
