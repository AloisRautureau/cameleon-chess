//
// Created by bitterboyy on 3/4/21.
//

#include "evaluation.h"

namespace Chameleon {
    namespace Evaluation {
        int eval(position &pos){
            int material{0};
            int placement{0};
            int total{0};
            int pawnsW = pos.m_plist[WHITE][PAWN].size();
            int pawnsB = pos.m_plist[BLACK][PAWN].size();
            int sq = inv;

            //We first evaluate the position based on a pure material perspective, also delaing with placement while we're at it
            for(int side = 0; side < 2; side++){
                for(int pieceType = PAWN; pieceType <= KING; pieceType++){
                    if(pieceType == KNIGHT){
                        material += (pos.m_plist[side][pieceType].size() * (m_pieceValue[pieceType] + knightPawnsValue[side ? pawnsB : pawnsW])) * (side ? -1 : 1);
                    }
                    else if(pieceType == ROOK){
                        material += (pos.m_plist[side][pieceType].size() * (m_pieceValue[pieceType] + rookPawnsValue[side ? pawnsB : pawnsW])) * (side ? -1 : 1);
                    }
                    else{
                        material += (pos.m_plist[side][pieceType].size() * m_pieceValue[pieceType]) * (side ? -1 : 1);
                    }

                    //For each adress, calculate the piece-square value
                    for(int i = 0; i < pos.m_plist[side][pieceType].size(); i++){
                        sq = pos.m_plist[side][pieceType].get(i);
                        if(side) sq ^= 0x70;
                        placement += pcsqTable[pieceType][sq] * (side ? -1 : 1);
                    }
                }
            }

            //Calculate bonuses
            if(pos.m_plist[WHITE][BISHOP].size() > 1) material += bishopPair;
            if(pos.m_plist[BLACK][BISHOP].size() > 1) material -= bishopPair;
            if(pos.m_plist[WHITE][KNIGHT].size() > 1) material += knightPair;
            if(pos.m_plist[BLACK][KNIGHT].size() > 1) material -= knightPair;
            if(pos.m_plist[WHITE][ROOK].size() > 1) material += rookPair;
            if(pos.m_plist[BLACK][ROOK].size() > 1) material -= rookPair;

            if(!pawnsW) material += noPawnPenalty;
            if(!pawnsB) material -= noPawnPenalty;

            total += (material + placement) * (pos.m_side ? -1 : 1);

            //If we're in a draw position, just cut the eval altogether and return 0;
            //if(drawDetection(pos, material)) return 0;
            return total;
        }

        bool drawDetection(position &pos, int materialScore){
            //If both sides have no pawns, one side needs a +4 pawns material advantage in order to not draw
            if(!pos.m_plist[WHITE][PAWN].size() && !pos.m_plist[BLACK][PAWN].size()) return (materialScore < 400 && materialScore > -400);
            return false;
        }

        int see(const position &pos, movebits move, bool side) {
            //We first need to find every potential attackers to the square, making sure they aren't pinned !
            pieceList wAttackers{{}};
            pieceList bAttackers{{}};
            int scores[32];
            int scoreIndex = 0;
            int from = position::fromSq(move);
            int to = position::toSq(move);
            int attackerDelta = 0;

            //Check for the bishop deltas
            for (auto delta : m_pieceDelta[BISHOP]) {
                if (!delta) break;
                //While we check for diagonnaly attacking pieces, might as well take care of pawns/kings
                if (pos.m_pieces[to + delta] == PAWN || pos.m_pieces[to + delta] == KING) {
                    if (to + delta == from) {
                        attackerDelta = delta;
                        continue;
                    }
                    if (pos.m_color[to + delta] == WHITE) wAttackers.add(to + delta);
                    else bAttackers.add(to + delta);
                    continue;
                }

                for (int currSquare = to + delta; !(currSquare & 0x88); currSquare += delta) {
                    if (pos.m_pieces[currSquare] == BISHOP || pos.m_pieces[currSquare] == QUEEN) {
                        if (currSquare == from) {
                            attackerDelta = delta;
                            break;
                        }
                        if (pos.m_color[currSquare] == WHITE) wAttackers.add(currSquare);
                        else bAttackers.add(currSquare);
                        break;
                    } else if (pos.m_pieces[currSquare] != EMPTY) break;
                }
            }

            //Same thing for rook
            for (auto delta : m_pieceDelta[ROOK]) {
                if (!delta) break;
                //Here we just need to check for king moves
                if (pos.m_pieces[to + delta] == KING) {
                    if (to + delta == from) {
                        attackerDelta = delta;
                        continue; //We don't add the original attacker to the plist
                    }
                    if (pos.m_color[to + delta] == WHITE) wAttackers.add(to + delta);
                    else bAttackers.add(to + delta);
                    continue;
                }

                for (int currSquare = to + delta; !(currSquare & 0x88); currSquare += delta) {
                    if (pos.m_pieces[currSquare] == ROOK || pos.m_pieces[currSquare] == QUEEN) {
                        if (currSquare == from) {
                            attackerDelta = delta;
                            break;
                        }
                        if (pos.m_color[currSquare] == WHITE) wAttackers.add(currSquare);
                        else bAttackers.add(currSquare);
                        break;
                    } else if (pos.m_pieces[currSquare] != EMPTY) break;
                }
            }


            //Finally, same for knights
            for (auto delta : m_pieceDelta[KNIGHT]) {
                if (!delta) break;
                if (pos.m_pieces[to + delta] == KNIGHT) {
                    if (to + delta == from) {
                        attackerDelta = delta;
                        break;
                    }
                    if (pos.m_color[to + delta] == WHITE) wAttackers.add(to + delta);
                    else bAttackers.add(to + delta);
                }
            }

            //Now we've got every direct attacker, we can simulate the initial capture
            int capturedValue = m_pieceValue[pos.m_pieces[to]];
            int attackerValue = m_pieceValue[pos.m_pieces[from]];
            scores[scoreIndex++] = capturedValue;
            side ^= 1;

            char attacker = pos.m_pieces[from];
            int attackerSquare = from;
            //We'll repeat this for every attacker
            do {
                //The attacker is now the captures piece
                capturedValue = attackerValue;

                //We now add hidden attackers after the first move, those can be found by following the attackerDelta from the from square
                if (pos.m_pieces[attacker] != KNIGHT && pos.m_pieces[attacker] != KING && scoreIndex < 32) {
                    for (int currSquare = from + attackerDelta; !(currSquare & 0x88); currSquare += attackerDelta) {
                        if (pos.m_pieces[currSquare] != EMPTY) {
                            //Check if the piece found can use the delta
                            int attack = pos.m_attackArray[to - currSquare + 128];
                            switch (pos.m_pieces[currSquare]) {
                                case BISHOP:
                                    if (attack == position::attKQBbP || attack == position::attKQBwP ||
                                        attack == position::attQB) {
                                        if (pos.m_color[currSquare] == WHITE) wAttackers.add(currSquare);
                                        else bAttackers.add(currSquare);
                                    }
                                    break;
                                case ROOK:
                                    if (attack == position::attKQR || attack == position::attQR) {
                                        if (pos.m_color[currSquare] == WHITE) wAttackers.add(currSquare);
                                        else bAttackers.add(currSquare);
                                    }
                                    break;
                                case QUEEN:
                                    if (attack != position::attNONE && attack != position::attN) {
                                        if (pos.m_color[currSquare] == WHITE) wAttackers.add(currSquare);
                                        else bAttackers.add(currSquare);
                                    }
                                    break;
                                default:
                                    break;
                            }
                            break;
                        }
                    }
                }
                if (bAttackers.size() < 1 || wAttackers.size() < 1) break;

                //Now, we find the least valuable attacking piece, and make it capture the square
                int size = side ? bAttackers.size() : wAttackers.size();
                for (int i = 0; i < size; i++) {
                    if (i == 0 ||
                        m_pieceValue[pos.m_pieces[side ? bAttackers.get(i) : wAttackers.get(i)]] < attackerValue) {
                        attackerSquare = side ? bAttackers.get(i) : wAttackers.get(i);
                        attacker = pos.m_pieces[attackerSquare];
                        attackerValue = m_pieceValue[attacker];
                    }
                }

                // We should remove the attacker since it won't be attacking anymore
                if (!side && wAttackers.size() > 0) wAttackers.remove(attackerSquare);
                else if(bAttackers.size() > 0) bAttackers.remove(attackerSquare);

                //And update the score
                scores[scoreIndex] = capturedValue - scores[scoreIndex - 1];
                scoreIndex++;
                side ^= 1;
            } while (bAttackers.size() > 0 && wAttackers.size() > 0);

            //Now evaluate the whole sequence
            while (scoreIndex > 1) {
                scoreIndex--;
                if (scores[scoreIndex - 1] > -scores[scoreIndex]) {
                    scores[scoreIndex - 1] = -scores[scoreIndex];
                }
            }

            return scores[0];
        }
    }
}