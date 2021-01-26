//
// Created by bitterboyy on 1/26/21.
//

#ifndef BAUB_CHESS_EVALUATION_H
#define BAUB_CHESS_EVALUATION_H

#include "Board.h"

/*
 * The Evaluation class serves the purpose of approximating the winning side base on material, piece-square bonuses or pawn structure
 * It will mainly be used by the Search functions, and is kept in it's own file for clarity
 */

class Evaluation{
private:
    Board& board;

public:
    explicit Evaluation(Board& board) : board(board){};

    //A symetric evaluation function (sideToMove is max)
    int evaluation(){
        //Considers pure material as well as piece-square value first
        int material[2] = {0, 0};
        int pcSq[2] = {0, 0};

        for(int side = 0; side < 2; side++){
            for(int pieceType = 0; pieceType < 6; pieceType++){
                for(int piece = 0; piece < 10; piece++){
                    if((side == WHITE ? board.whitePieces[pieceType][piece] : board.blackPieces[pieceType][piece]) == INV) break;
                    material[side] += pieceValue[pieceType];
                    pcSq[side] += pieceSquareTables[pieceType][side == WHITE ? invSquare8x8(board.whitePieces[pieceType][piece]) : square8x8(board.blackPieces[pieceType][piece])];
                }
            }
        }

        //TODO pawn struct eval with zobrist hashes

        if(board.sideToMove == WHITE){
            return (material[BLACK] + pcSq[BLACK]) - (material[WHITE] + pcSq[WHITE]);
        }
        else return (material[WHITE] + pcSq[WHITE]) - (material[BLACK] + pcSq[BLACK]);
    }
};

#endif //BAUB_CHESS_EVALUATION_H
