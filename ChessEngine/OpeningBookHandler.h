//
// Created by bitterboyy on 28/01/2021.
//

#ifndef BAUB_CHESS_OPENINGBOOKHANDLER_H
#define BAUB_CHESS_OPENINGBOOKHANDLER_H

#include <iostream>
#include <fstream>
#include <utility>
#include "Constants.h"

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
//A book entry takes up 256 bits of space
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
    std::string pathToBook;

    //The book handler loads the whole book in memory to ease up management
    //Sure, it's kinda slow and certinely not optimized, but since book generation will likely be a one time occurence,
    //execution speed is not a primary concern. Correctness, on the other hand, is.
    bookEntry openingBook[100000];

public:
    explicit BookHandler(std::string path): pathToBook(std::move(path)){
        int i = 0;
        std::ifstream book(pathToBook);
        while(!book.eof()){
                openingBook[i] = readEntry(i);
        }
        book.close();
    };

    //CLI interface to interact with the book, modifying, adding, generating, deleting line, etc etc
    int openingBookWizard(){
        std::cout << "#### Welcome to the Opening Book Wizard of SCP ! ####" << std::endl << std::endl;

        std::cout << "Choose one of the options listed below to start : " << std::endl;
        std::cout << "(1) Generate from game collection\n"
                     "(2) Manage the current book\n"
                     "(3) Quit\n" << std::endl;

        while(true){
            int choice = 0;
            std::cin >> choice;

            switch(choice){
                case 1:
                    std::cout << "Do you want to regenerate the book entirely (1) or add lines (2) ? : ";
                    std::cin >> choice;

                    switch(choice){
                        case 1:
                            //Choose a file and generate book from it
                            break;
                        case 2:
                        default:
                            //Choose a file and add lines to the current book
                            break;
                    }
                    break;

                case 2:
                    //Here we can delete or change lines (finds the move we want to change, and modifies it)
                    //Or add an entirely new line (input the move where it changes, the program finds it's index, and goes to it's last sibling
                    //then changes it's nextSibling variable, before adding the new line after the last index, then continues from here)
                    break;

                case 3:
                    return 0;

                default:
                    std::cout << "Please enter a valid choice" << std::endl;
            }
        }

        return 0;
    }

    //Reads the entry at the given index in the book
    bookEntry readEntry(int index){
        //First we need to open the file
        std::ifstream bookFile;
        bookFile.open(pathToBook, std::ios::binary);
        //It will open the file from the root index (index 0), which is e4 since it is the most common first move
        //A memory block size will be 256bits here (one memory block = 1 book entry)
        bookEntry bookMove{};
        char* buffer = (char*) &bookMove;

        if(!bookFile){
            std::cout << "The book file could not be read" << std::endl;
            return bookMove;
        }

        //Placing the head on the entry we want to read
        bookFile.seekg(sizeof(bookEntry)*index);
        bookFile.read(buffer, sizeof(bookEntry));

        //delete[] buffer;
        return bookMove;
    }

    //Writes the given entry at the current index in the book
    void writeEntry(int index, bookEntry entry){
        //Same thing but here we need to write
        std::ofstream bookFile;
        bookFile.open(pathToBook, std::ios::binary);
        char* buffer = (char*) &entry;

        if(!bookFile){
            std::cout << "The book file could not be read" << std::endl;
            return;
        }

        //Placing the head on the entry we want to read
        bookFile.seekp(sizeof(bookEntry)*index);
        bookFile.write(buffer, sizeof(bookEntry));

        //delete[] buffer;
    }

};


#endif //BAUB_CHESS_OPENINGBOOKHANDLER_H
