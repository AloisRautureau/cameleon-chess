//
// Created by bitterboyy on 3/10/21.
//

#include "cli.h"

void cli::showLogo() {
    std::cout << logo << std::endl << std::endl;
}

int cli::mainMenu(board_representation &board) {
    std::cout << "1) Play a game!\n"
                 "2) Debug menu\n"
                 "3) Quit\n" << std::endl;

    std::cout << "Enter your choice here -> ";
    int choice = 0;
    std::cin >> choice;

    switch(choice){
        case 0:
        default:
            std::cout << "Please enter a valid option" << std::endl << std::endl;
            mainMenu(board);
            break;

        case 1:
            play(board);
            break;

        case 2:
            //debugMenu(board);
            break;

        case 3:
            std::cout << "Bye bye!" << std::endl;
            abort();
    }

    return 0;
}

int cli::play(board_representation &board) {
    int playerSide = WHITE;
    int cpuSide = BLACK;
    int cpuDepth = 3;

    std::string sideChoice;
    std::string depthChoice;

    std::cout << "Any prefered side ? (white/black/surprise) -> ";
    std::cin >> sideChoice;
    std::cout << "What about CPU max search depth ? (enter any number, moves may take a really long time past depth 5) -> ";
    std::cin >> depthChoice;


    if(sideChoice == "white"){
    }
    else if(sideChoice == "black"){
        playerSide = BLACK;
        cpuSide = WHITE;
    }
    else{
        srand(time(NULL));
        int side = rand() % 1 + 1;
        playerSide = side != 1;
        cpuSide = !playerSide;
    }

    cpuDepth = std::stoi(depthChoice);

    std::cout << "You'll be playing the " << (playerSide ? "BLACK" : "WHITE") << " pieces against a depth " << cpuDepth << " CPU, good luck!" << std::endl;

    //While moves are available to us, we can continue playing
    //Careful tho as we may still have illegal moves, so we ought to check for those
    board.gen();
    while(board.m_moveStackIndex){
        display::showPosition(board);
        if(board.m_side == playerSide){
            std::string playerMove;
            movebits parsedMove;
            bool legal = false;
            while(!legal){
                std::cin >> playerMove;
                parsedMove = parseUserInput(playerMove);
                std::cout << display::displayMove(parsedMove) << std::endl;
                for(int i = 0; i < board.m_moveStackIndex; i++){
                    std::cout << display::displayMove(board.m_moveStack[i]) << std::endl;
                    if(board.m_moveStack[i] == parsedMove){
                        legal = true;
                        break;
                    }
                }
                if(!legal) std::cout << "The move entered is illegal" << std::endl;
            }
            board.make(parsedMove);
        }
        else{
            board.make(search::bestMove(board, cpuDepth));
        }
        board.gen();
    }

    if(board.checkmate()){
        return !board.m_side;
    }

    return -1;
}

movebits cli::parseUserInput(std::string userInput) {
    if(userInput.size() != 7) return 0;
    std::string fromInput;
    std::string toInput;
    fromInput += userInput[0];
    fromInput += userInput[1];
    toInput += userInput[2];
    toInput += userInput[3];

    int from = display::squareToIndex(fromInput);
    int to = display::squareToIndex(toInput);
    std::cout << std::hex << from << "->" << to << std::dec << std::endl;
    char flag = (userInput[5]-'0')*10 + userInput[6]-'0';
    return board_representation::encodeMove(from, to, flag);
}
