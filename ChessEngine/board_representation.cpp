//
// Created by bitterboyy on 2/20/21.
//

#include "board_representation.h"

void board_representation::gen(){
    m_moveStackIndex = 0;
    for(int pieceType = 0; pieceType < 6; pieceType++){
        for(sq adress : m_pieces[m_sideToMove][pieceType]){
            if(pieceType == PAWN){
                //We need two different pieces of code depending on which side the pawn belongs to since they can only
                //move forward
                if(m_sideToMove == WHITE){
                    /*
                     * Pawns can either :
                     * - push one square if no piece is on target
                     * - push two squares if they stand on their original rank and no piece is obstructing
                     * - one square diagonnaly forward if an opposite piece sits there
                     */
                    if(m_piecesBoard[adress + N] == EMPTY){
                        if(m_piecesBoard[adress + (2*N)] == EMPTY && rank(adress) == 1){ //Double push is available
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+(2*N)), DPAWNPUSH);
                            m_moveStackIndex++;
                        }
                        if(rank(adress) == 6){ //That would be a promotion
                            for(int i = 0; i < 4; i++){
                                m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+N), flag(NPROM+i));
                                m_moveStackIndex++;
                            }
                        }
                        else{
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+N), QUIET);
                            m_moveStackIndex++;
                        }
                    }

                    if(m_colorBoard[adress + NW] == BLACK){ //Capture to the north west
                        if(rank(adress) == 6){ //promo capture case
                            for(int i = 0; i < 4; i++){
                                m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+NW), flag(NPROMCAP+i));
                                m_moveStackIndex++;
                            }
                        }
                        else{
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+NW), CAP);
                            m_moveStackIndex++;
                        }
                    }
                    if(m_colorBoard[adress + NE] == BLACK){ //Capture to the north east
                        if(rank(adress) == 6){ //promo capture case
                            for(int i = 0; i < 4; i++){
                                m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+NE), flag(NPROMCAP+i));
                                m_moveStackIndex++;
                            }
                        }
                        else{
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+NE), CAP);
                            m_moveStackIndex++;
                        }
                    }
                }
                else{
                    //Same thing as the above code but mirrored
                    if(m_piecesBoard[adress + S] == EMPTY){
                        if(m_piecesBoard[adress + (2*S)] == EMPTY && rank(adress) == 6){ //Double push is available
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+(2*S)), DPAWNPUSH);
                            m_moveStackIndex++;
                        }
                        if(rank(adress) == 1){ //That would be a promotion
                            for(int i = 0; i < 4; i++){
                                m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+S), flag(NPROM+i));
                                m_moveStackIndex++;
                            }
                        }
                        else{
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+S), QUIET);
                            m_moveStackIndex++;
                        }
                    }

                    if(m_colorBoard[adress + SW] == WHITE){ //Capture to the north west
                        if(rank(adress) == 1){ //promo capture case
                            for(int i = 0; i < 4; i++){
                                m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+SW), flag(NPROMCAP+i));
                                m_moveStackIndex++;
                            }
                        }
                        else{
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+SW), CAP);
                            m_moveStackIndex++;
                        }
                    }
                    if(m_colorBoard[adress + SE] == WHITE){ //Capture to the north east
                        if(rank(adress) == 1){ //promo capture case
                            for(int i = 0; i < 4; i++){
                                m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+SE), flag(NPROMCAP+i));
                                m_moveStackIndex++;
                            }
                        }
                        else{
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, sq(adress+SE), CAP);
                            m_moveStackIndex++;
                        }
                    }
                }
            }

            //Next we can deal with sliding piece generation
            else{
                //Sliding pieces use rays to check when they sould stop being able to move in a certain direction
                //to make that happen, we make one step in every given direction until we encounter an obstacle
                //If said obstacle is one of our own pieces or out of the board shenanigans, we can't make the last step
                //Otherwise, the last step is a capture
                for(auto stepDirection : pieceMoves[pieceType]){
                    if(stepDirection == 0) break;

                    sq currentSquare = adress;
                    bool obstacleFound = false;
                    while(!obstacleFound){
                        currentSquare = sq(currentSquare + stepDirection);
                        if(m_colorBoard[currentSquare] == m_sideToMove
                            || (currentSquare & 0x88)) obstacleFound = true;
                        else if(m_colorBoard[currentSquare] == !m_sideToMove){
                            obstacleFound = true;
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, currentSquare, CAP);
                            m_moveStackIndex++;
                        }
                        else{
                            m_moveStack[m_moveStackIndex] = encodeMove(adress, currentSquare, QUIET);
                            m_moveStackIndex++;
                            obstacleFound = !pieceMoves[0][pieceType];
                        }
                    }
                }
            }
        }
    }

    //Lastly, we can deal with castling
    /*
     * The rules here are as follow :
     * - King must not be in check
     * - The squares between king/rook should not be occupied OR attacked
     * - castling rights must allow the move
     */
    if(m_sideToMove){
        if(m_castlingRights & WKCASTLE
           && !m_piecesBoard[f1] && !m_piecesBoard[g1] && !m_piecesBoard[e1]
           && !sqAttacked(f1, BLACK) && !sqAttacked(g1, BLACK) && !sqAttacked(e1, BLACK)){
            m_moveStack[m_moveStackIndex] = encodeMove(e1, g1, KCASTLE);
            m_moveStackIndex++;
        }
        if(m_castlingRights & WQCASTLE
           && !m_piecesBoard[d1] && !m_piecesBoard[c1] && !m_piecesBoard[b1] && !m_piecesBoard[e1]
           && !sqAttacked(d1, BLACK) && !sqAttacked(c1, BLACK) && !sqAttacked(b1, BLACK) && !sqAttacked(e1, BLACK)){
            m_moveStack[m_moveStackIndex] = encodeMove(e1, c1, QCASTLE);
            m_moveStackIndex++;
        }
    }
    else {
        if (m_castlingRights & BKCASTLE
            && !m_piecesBoard[f8] && !m_piecesBoard[g8] && !m_piecesBoard[e8]
            && !sqAttacked(f8, WHITE) && !sqAttacked(g8, WHITE) && !sqAttacked(e8, WHITE)) {
            m_moveStack[m_moveStackIndex] = encodeMove(e8, g8, KCASTLE);
            m_moveStackIndex++;
        }
        if (m_castlingRights & BQCASTLE
            && !m_piecesBoard[d8] && !m_piecesBoard[c8] && !m_piecesBoard[b8] && !m_piecesBoard[e8]
            && !sqAttacked(d8, WHITE) && !sqAttacked(c8, WHITE) && !sqAttacked(b8, WHITE) && !sqAttacked(e8, WHITE)) {
            m_moveStack[m_moveStackIndex] = encodeMove(e8, c8, QCASTLE);
            m_moveStackIndex++;
        }
    }

    for(movebits move : m_moveStack){
        if(!move) break;
        std::cout << move << std::endl;
    }
}

bool board_representation::sqAttacked(int sq, bool side) {
    for(int pieceType = 0; pieceType < 6; pieceType++){
        for(auto& adress : m_pieces[side][pieceType]){
            if(pieceType == PAWN){
                //Just check if the given square is in the diagonal of any of the opponents pawns
                if(side && m_colorBoard[adress + NW] == sq || m_colorBoard[adress + NE] == sq) return true;
                if(!side && m_colorBoard[adress + SW] == sq || m_colorBoard[adress + SE] == sq) return true;
            }

            //Here we can optimize by noticing we don't actually care about directions which aren't going anywhere near
            //our given square.
            else{
                for(auto stepDirection : pieceMoves[pieceType]){
                    if((sq - adress < 0) == (stepDirection < 0) && (sq - adress)%stepDirection) continue;
                    if(stepDirection == 0) break;

                    int currentSquare  = adress;
                    bool obstacleFound = false;
                    while(!obstacleFound){
                        currentSquare += stepDirection;
                        if(m_colorBoard[currentSquare] != EMPTY || !pieceMoves[0][pieceType]) {
                            obstacleFound = true;
                            if(currentSquare == sq) return true;
                        }
                        if(currentSquare & 0x88) obstacleFound = true;
                        else if(currentSquare == sq) return true;
                    }
                }
            }
        }
    }
    return false;
}
