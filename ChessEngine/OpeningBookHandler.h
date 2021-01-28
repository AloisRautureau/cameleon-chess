//
// Created by bitterboyy on 28/01/2021.
//

#ifndef BAUB_CHESS_OPENINGBOOKHANDLER_H
#define BAUB_CHESS_OPENINGBOOKHANDLER_H

/*
 * A completely separate program from the main chess engine, used to manage the opening book
 * It's purpose is to add, remove or modify entries easily through the command line
 * It affects the openingBook file
 *
 * The opening book uses the Arena's Book format (ABK), altough I made some modifications
 * It stores an array of structures to a binary file
 * The structure of an entry can be described as such :
 * struct BookMoveEntry {
 * char from;
 * char to;
 * int ngames;
 * int nwon;
 * int nlost;
 * int plycount;        //Move order
 * int nextMove;        //Index of the next move in the current line
 * int nextSibling;     //Let's us swap to a different line if necessary
 * }
 */

int openingBookWizard(){

    return 0;
}

#endif //BAUB_CHESS_OPENINGBOOKHANDLER_H
