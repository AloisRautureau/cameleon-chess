//
// Created by bitterboyy on 1/3/21.
//

#ifndef BAUB_CHESS_UTILITY_H
#define BAUB_CHESS_UTILITY_H

#include <string>
#include <vector>

std::vector<std::string> split(std::string toSplit, char splitter){
    std::vector<std::string> substrings;
    std::string substring;
    for(int i = 0; i <= toSplit.length(); i++){
        if(toSplit[i] == splitter || i == toSplit.length()){
            substrings.push_back(substring);
            substring = "";
        }

        else{
            substring += toSplit[i];
        }
    }
    return substrings;
}

int pieceCorrespondance(int piece, int color){
    int pieceTransf;
    switch(piece){
        case 1:
            pieceTransf = color == 1 ? 0 : 6;
            break;
        case 3:
            pieceTransf = color == 1 ? 1 : 7;
            break;
        case 4:
            pieceTransf = color == 1 ? 2 : 8;
            break;
        case 5:
            pieceTransf = color == 1 ? 3 : 9;
            break;
        case 9:
            pieceTransf = color == 1 ? 4 : 10;
            break;
        case 100:
            pieceTransf = color == 1 ? 5 : 11;
            break;

        default:
            pieceTransf = 0;
            break;
    }

    return pieceTransf;
}

#endif //BAUB_CHESS_UTILITY_H
