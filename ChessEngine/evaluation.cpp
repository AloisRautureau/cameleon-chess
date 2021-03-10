//
// Created by bitterboyy on 3/4/21.
//

#include "evaluation.h"

evaluation::evaluation() {
    for(int side = WHITE; side < 2; side++){
        for(int piece = PAWN; piece < 6; piece++){
            for(int sq = 0; sq < 0x88; sq++){
                if(!side){
                    mg_tables[side][piece][sq] = m_pieceValueMG[piece] + mg_plist[piece][sq];
                    eg_tables[side][piece][sq] = m_pieceValueEG[piece] + eg_plist[piece][sq];
                }
                else{
                    mg_tables[side][piece][sq] = m_pieceValueMG[piece] + mg_plist[piece][sq^0x70];
                    eg_tables[side][piece][sq] = m_pieceValueEG[piece] + eg_plist[piece][sq^0x70];
                }
            }
        }
    }
}

int evaluation::eval(board_representation &board) {
    //The idea between tappered evaluation is to interpolate endgame and midgame evalution depending on the pieces
    //present on the board (they determine gamephase sort of)
    int midgameEval[2]{0};
    int endgameEval[2]{0};
    int gamephase = 0;

    //Evaluate each piece in an endgame and midgame situation
    int sq{0};
    for(int side = WHITE; side < 2; side++){
        for(int pieceType = PAWN; pieceType < 6; pieceType++){
            for(int index = 0; index < board.m_plist[side][pieceType].size(); index++){
                sq = board.m_plist[side][pieceType].get(index);
                midgameEval[side] += mg_tables[side][pieceType][sq];
                endgameEval[side] += eg_tables[side][pieceType][sq];
                gamephase += gamephaseValue[pieceType];
            }
        }
    }

    //We can now interpolate the results based on the gamephase value
    int midgameScore = midgameEval[board.m_side] - midgameEval[!board.m_side];
    int endgameScore = endgameEval[board.m_side] - endgameEval[!board.m_side];
    int midgameProximity = gamephase; //How close to the midgame are we?
    if(midgameProximity > 24) midgameProximity = 24; //The max we can have with all initial pieces, we can go above this value with promotions
    int endgameProximity = 24 - midgameProximity;
    return (midgameScore * midgameProximity + endgameScore * endgameProximity);
}
