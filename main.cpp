#include <iostream>
#include "ChessEngine/BoardRep.h"

int main() {
    BoardRep board = BoardRep();

    do{
        board.showCurrentPosition();
        board.generatePseudoLegal();
        std::string movestr;
        bool legal = false;
        Move move(0, 0, 0);

        do{
        std::cin >> movestr;

        if(movestr.size() >= 5){
            int start = coordinateToInt(movestr.substr(0, 2));
            int end = coordinateToInt(movestr.substr(2, 2));
            int special = movestr.at(4) - '0';
            move = board.getMoveStack().searchMove(start, end);
            move.special = special;
            if(move.start == move.end) std::cout << "Ce move n'est pas légal!" << std::endl;
            else legal = true;
        } else {
            int start = coordinateToInt(movestr.substr(0, 2));
            int end = coordinateToInt(movestr.substr(2, 2));
            move = board.getMoveStack().searchMove(start, end);
            if(move.start == move.end) std::cout << "Ce move n'est pas légal!" << std::endl;
            else legal = true;
        }

        }while(!legal);

        board.makeMove(move);
    }while(board.checkmated(WHITE) == -1 || board.checkmated(BLACK) == -1);

    return 0;
}
