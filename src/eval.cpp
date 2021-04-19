//
// Created by bitterboyy on 4/19/21.
//

#include "eval.h"

using namespace Chameleon;

namespace Eval {
    int eval(position &pos){
        int pc_sq_midgame = pc_sqEval(pos, false);
        int material = materialEval(pos);

        return (pos.m_side ? -1 : 1) * (pc_sq_midgame + material);
    }


    int materialEval(position &pos){
        /*
         * TODO : add the other considerations (bishop pair, etc, etc)
         */
        //Loops through the piece list of the position, and multiplies the size of each by the value of the piece
        int material_score{0};
        for(int side = 0; side < 2; side++){
            for(int piece = 0; piece <= KING; piece++){
                material_score += (side ? -1 : 1) * (pos.m_plists[side][piece].size * pieceValue[piece]);
            }
        }

        return material_score;
    }

    int pc_sqEval(position &pos, bool endgame){
        int pc_sq_score{0};
        int square = 0;
        for(int side = 0; side < 2; side++){
            for(int piece_type = 0; piece_type < 6; piece_type++){
                for(int i = 0; i < pos.m_plists[side][piece_type].size; i++){
                    square = (side ? 0x70 : 0) ^ pos.m_plists[side][piece_type].indexes[i];
                    pc_sq_score += (side ? -1 : 1) * (endgame ? pcSqTablesEnd[piece_type][square] : pcSqTablesMiddle[piece_type][square]);
                }
            }
        }
        return pc_sq_score;
    }
}
