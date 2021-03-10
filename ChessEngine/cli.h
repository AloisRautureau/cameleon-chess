//
// Created by bitterboyy on 3/10/21.
//

#ifndef BAUB_CHESS_CLI_H
#define BAUB_CHESS_CLI_H


#include <string>
#include "board_representation.h"
#include "search.h"
#include "display.h"

static std::string logo = " ______     ______     ______   _____     ______     ______     __  __     ______    \n"
                          "/\\  ___\\   /\\  ___\\   /\\  == \\ /\\  __-.  /\\  ___\\   /\\  == \\   /\\ \\/\\ \\   /\\  ___\\   \n"
                          "\\ \\___  \\  \\ \\ \\____  \\ \\  _-/ \\ \\ \\/\\ \\ \\ \\  __\\   \\ \\  __<   \\ \\ \\_\\ \\  \\ \\ \\__ \\  \n"
                          " \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\____-  \\ \\_____\\  \\ \\_____\\  \\ \\_____\\  \\ \\_____\\ \n"
                          "  \\/_____/   \\/_____/   \\/_/     \\/____/   \\/_____/   \\/_____/   \\/_____/   \\/_____/ \n"
                          "                                                                                     \n"
                          "";

class cli {
public:
    static void showLogo();

    static int mainMenu(board_representation &board);

    static int debugMenu(board_representation &board);

    static movebits parseUserInput(std::string userInput);

    static int play(board_representation &board);
};


#endif //BAUB_CHESS_CLI_H
