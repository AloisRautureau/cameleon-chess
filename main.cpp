#include <iostream>
#include "ChessEngine/BoardRep.h"
#include "ChessEngine/Engine.h"

int play(int engineSide, bool engineOnly, BoardRep board, Engine engine){
    Move move = Move(0, 0, 0);
    bool legal;

    board.showCurrentPosition();

    do{
        if(engineSide == board.getSide()) move = engine.searchRoot(board.getSide());
        if(engineOnly) move = engine.searchRoot(board.getSide());
        else{
            

            std::cin >> humanMove;

            if(humanMove.size() >= 5){

            }
        }

    }while(!legal)
}

int main() {
    BoardRep board = BoardRep();
    Engine engine = Engine(3, &board);


    return 0;
}
