//
// Created by bitterboyy on 28/01/2021.
//

#ifndef BAUB_CHESS_OPENINGBOOKHANDLER_H
#define BAUB_CHESS_OPENINGBOOKHANDLER_H

#include <fstream>
#include <utility>
#include "Utility.h"

/*
 * A completely separate program from the main chess engine, used to manage the opening book
 * It's purpose is to add, remove or modify entries easily through the command line
 * It affects the openingBook file
 *
 * The opening book uses the Arena's Book format (ABK), altough I made some modifications
 * It stores an array of structures to a binary file
 *
 * The binary file can be describde as cut in chunks of 256bits, each chunk containing a book entry
 */
struct bookEntry {
    MOVEBITS move;
    int games;
    int wins;
    int losses;
    int draws;
    int ply;
    int nextMove;
    int nextSibling;
};

class BookHandler{
private:
    const int CHUNKSIZE = 256;
    std::fstream bookFile;
    bool fileError = false;

public:
    BookHandler(std::string path){
        bookFile = std::fstream(path, std::ios::in | std::ios::out | std::ios::binary);
        fileError = !bookFile.is_open();
    }
};

#endif //BAUB_CHESS_OPENINGBOOKHANDLER_H
