#include <iostream>
#include "ChessEngine/position.h"
#include "ChessEngine/display.h"
#include "ChessEngine/evaluation.h"
#include "ChessEngine/uci.h"

using namespace Chameleon;

int main() {
    system("clear");

    position pos;

    display::showPosition(pos);

    Evaluation::init();

    UCI::uciListen(pos);

    return 0;
}
