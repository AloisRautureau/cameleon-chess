//
// Created by bitterboyy on 4/16/21.
//

#include <iostream>
#include "position.h"

namespace Chameleon {
    position::position() {
        //Here, we initialize every variable to be equivalent to the initial position of a chess game
        //Notably, we initialize arrays here

        for(int i = 0; i < 0x88; i++){
            m_board[i] = initialPosition[i];
        }

        init_plist(m_plists[Wh][P], {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17});
        init_plist(m_plists[Bl][P], {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67});
        init_plist(m_plists[Wh][N], {0x01, 0x06});
        init_plist(m_plists[Bl][N], {0x71, 0x76});
        init_plist(m_plists[Wh][B], {0x02, 0x05});
        init_plist(m_plists[Bl][B], {0x72, 0x75});
        init_plist(m_plists[Wh][R], {0x00, 0x07});
        init_plist(m_plists[Bl][R], {0x70, 0x77});
        init_plist(m_plists[Wh][Q], {0x03});
        init_plist(m_plists[Bl][Q], {0x73});
        init_plist(m_plists[Wh][K], {0x04});
        init_plist(m_plists[Bl][K], {0x74});
    }

    void position::gen(movestack &stack) {
        int from;
        int raySq;
        int pinDelta;
        //Traverse every piece list of the side to move, then for each piece, get it's type and generate corresponding moves
        for (int piece_type = P; piece_type < K + 1; piece_type++) {
            for (int i = 0; i < m_plists[m_side][piece_type].size; i++) {
                from = m_plists[m_side][piece_type].indexes[i];
                pinDelta = getPinDelta(m_pinned, from); //If this is equal to 0 the piece can move freely. Otherwise it is limited to its delta
                if (piece_type == P) {
                    //Pawns can only move up for white, down for black, capture diagonally, and promote
                    //We deal with each of those in two branches, one for each color, to keep things clean
                    if (!m_side) {
                        if (m_board[from + 0x10] == EMPTY && !pinDelta%0x10) {
                            if (from >= 60 && from <= 67)
                                storeMove(from, from + 0x10, NPROM, stack); //Quiet promotion
                            else {
                                storeMove(from, from + 0x10, QUIET, stack); //Pure quiet
                                if (m_board[from + 0x20] == EMPTY) {
                                    storeMove(from, from + 0x20, DPAWNPUSH, stack); //Double pawn push
                                }
                            }
                        }

                        if (m_board[from + 0x11] & BLACK && !pinDelta%0x11) {
                            if (from >= 60 && from <= 67)
                                storeMove(from, from + 0x11, NPROMCAP, stack); //Capture promotion
                            else storeMove(from, from + 0x11, CAP, stack);
                        }
                        else if (from + 0x11 == m_ep && !pinDelta%0x11 && !epGotcha(from, from+1)) storeMove(from, from + 0x11, EPCAP, stack);

                        if (m_board[from + 0x09] & BLACK && !pinDelta%0x09) {
                            if (from >= 60 && from <= 67)
                                storeMove(from, from + 0x09, NPROMCAP, stack); //Capture promotion
                            else storeMove(from, from + 0x09, CAP, stack);
                        }
                        else if (from + 0x09 == m_ep && !pinDelta%0x09 && !epGotcha(from, from-1)) storeMove(from, from + 0x09, EPCAP, stack);

                    } else {
                        if (m_board[from - 0x10] == EMPTY && !pinDelta%0x10) {
                            if (from >= 10 && from <= 17)
                                storeMove(from, from - 0x10, NPROM, stack); //Quiet promotion
                            else {
                                storeMove(from, from - 0x10, QUIET, stack); //Pure quiet
                                if (m_board[from - 0x20] == EMPTY) {
                                    storeMove(from, from - 0x20, DPAWNPUSH, stack); //Double pawn push
                                }
                            }
                        }

                        if (m_board[from - 0x11] & WHITE && !pinDelta%0x11) {
                            if (from >= 10 && from <= 17)
                                storeMove(from, from - 0x11, NPROMCAP, stack); //Capture promotion
                            else storeMove(from, from - 0x11, CAP, stack);
                        }
                        else if (from - 0x11 == m_ep && !pinDelta%0x11 && !epGotcha(from, from-1)) storeMove(from, from - 0x11, EPCAP, stack);

                        if (m_board[from - 0x09] & WHITE && !pinDelta%0x09) {
                            if (from >= 10 && from <= 17)
                                storeMove(from, from - 0x09, NPROMCAP, stack); //Capture promotion
                            else storeMove(from, from - 0x09, CAP, stack);
                        }
                        else if (from - 0x09 == m_ep && !pinDelta%0x09 && !epGotcha(from, from+1)) storeMove(from, from - 0x09, EPCAP, stack);
                    }
                }

                //The king has to get particular attention when writing a legal move generator, as his moves
                //are the most likely to be illegal ones
                else if(piece_type == K) {
                    for(auto delta : piece_delta[piece_type]){
                        raySq = from + delta;
                        if(!isInvalid(raySq) && !(m_board[raySq] & (m_side ? BLACK : WHITE)) && !isAttacked(raySq, true)) {
                            if(m_board[raySq]) storeMove(from, raySq, QUIET, stack);
                            else storeMove(from, raySq, CAP, stack);
                        }
                    }
                }

                else {
                    //For other pieces, we iterate through each direction, and follow the ray
                    //A ray stops when :
                    // - it hits a piece (if it's of an opposite color, we add the move as a capture, else don't add it)
                    // - it hits an invalid square
                    // - it can't slide
                    for(auto delta : piece_delta[piece_type]){
                        if(!delta || pinDelta%delta) break;
                        raySq = from;
                        for(;;){
                            raySq += delta;
                            if(isInvalid(raySq) || m_board[raySq] & (m_side ? BLACK : WHITE)) break;
                            if(m_board[raySq] & (m_side ? WHITE : BLACK)) { //Current square is occupied by opposing piece
                                storeMove(from, raySq, CAP, stack);
                                break;
                            }
                            else storeMove(from, raySq, QUIET, stack);
                            if(!piece_delta[0][piece_type]) break;
                        }
                    }
                }
            }
        }

        // Generating castling moves
        // Rules of castling :
        // - Castling rights allow it (king and rook haven't been moved
        // - King not in check (but this generator shouldn't be called if the king is in check anyway)
        // - No squares between king from and to are attacked by the opponent
        if(!m_side && !m_checked){
            if(m_castling&0b1000 && !m_board[0x05] && !m_board[0x06] && !isAttacked(0x05) && !isAttacked(0x06)) storeMove(0x04, 0x06, KCASTLE, stack);
            if(m_castling&0b0100 && !m_board[0x03] && !m_board[0x02] && !m_board[0x01] && !isAttacked(0x03) && !isAttacked(0x02)) storeMove(0x04, 0x02, QCASTLE, stack);
        }
        else if(m_side && !m_checked){
            if(m_castling&0b0010 && !m_board[0x75] && !m_board[0x76] && !isAttacked(0x75) && !isAttacked(0x76)) storeMove(0x74, 0x76, KCASTLE, stack);
            if(m_castling&0b0001 && !m_board[0x73] && !m_board[0x72] && !m_board[0x71]&& !isAttacked(0x73) && !isAttacked(0x72)) storeMove(0x74, 0x72, QCASTLE, stack);
        }

        std::cout << stack.size << std::endl;
        for (int i = 0; i < stack.size; i++) {
            std::cout << fromSq(stack.moves[i]) << " " << toSq(stack.moves[i]) << " " << flag(stack.moves[i])
                      << std::endl;
        }
    }

    void position::genNoisy(movestack &stack) {

    }

    void position::genEvasion(movestack &stack) {

    }

    void position::make(movebyte move) {

    }

    void position::unmake() {

    }

    bool position::isAttacked(int square, bool kingxray) {
        int attacking;
        int delta;
        int raySq;
        //To find out if a square is attacked, loop through opponent's pieces and check if they have an attack delta towards given square
        //If an attack delta is found, follow it to check if it is obstructed or not
        for(int piece_type = P; piece_type < K+1; piece_type++){
            for(int i = 0; i < m_plists[!m_side][piece_type].size; i++){
                attacking = m_plists[!m_side][piece_type].indexes[i];
                delta = get_attackingdelta(square, attacking, piece_type, !m_side);
                if(delta){ //If we found an attacking delta, follow it (if knight, king or pawn, stop after one iteration)
                    if(piece_delta[0][piece_type]) { //Sliding piece
                        raySq = attacking;
                        for(;;){
                            raySq += delta;
                            if(raySq == square) return true;
                            if(isInvalid(raySq) || m_board[raySq] != EMPTY) {
                                if(!(kingxray && m_board[raySq] != (KING & (m_side ? BLACK : WHITE)))) break; //If we're in an xray attack check, ignore the king
                            }
                        }
                    }
                    else if(attacking + delta == square) return true;
                }
            }
        }
        return false;
    }

    void position::updatePins() {
        int attacker;
        int delta;
        int raySq;
        int king = m_plists[m_side][K].indexes[0]; //Get our king's square
        int pinnedSq = 0;
        pins pinned;
        pins pinning;
        //The idea is to find every opponent piece that can get to the king, then follow their rays
        //We xray to the first piece found if it is one of our pieces, but stop if we encounter a second one
        for(int piece_type = P; piece_type < K+1; piece_type++){
            for(int i = 0; i < m_plists[!m_side][piece_type].size; i++){
                attacker = m_plists[!m_side][piece_type].indexes[i];
                delta = get_attackingdelta(king, attacker, piece_type, !m_side);
                if(!delta) continue;
                pinnedSq = 0;
                //In case we're dealing with a sliding piece, follow the ray stopping at second friendly piece found
                //or any enemy piece
                if(piece_delta[0][piece_type]) {
                    raySq = attacker;
                    for(;;) {
                        raySq += delta;
                        if(raySq == king) { //If we get to the king, it means a piece got pinned
                            addPin(pinned, pinnedSq, delta);
                            addPin(pinning, attacker, delta);
                            break;
                        }
                        if(m_board[raySq]){
                            if(m_board[raySq] & (m_side ? BLACK : WHITE)){ //Friendly piece on square
                                if(!pinnedSq) pinnedSq = raySq;
                                else break;
                            }
                            else break; //Not friendly pieces
                        }
                    }
                }
            }
        }

        m_pinned = pinned;
        m_pinning = pinning;
    }

    bool position::epGotcha(int epSquare, int captured) {
        //We must check, when generating an en passant move, if removing the two pawns will put our king in check
        //To do that, go on horizontal deltas from king position, and stop on any piece which isn't any of the two pawns
        //If the piece found isn't friendly and can attack horizontally, the en passant move is illegal
        int king = m_plists[m_side][K].indexes[0];
        int raySq;
        for(int delta = -1; delta < 2; delta += 2){
            raySq = king;
            for(;;){
                raySq += delta;
                if(isInvalid(raySq) || (m_board[raySq] & (m_side ? BLACK : WHITE) && raySq != epSquare && raySq != captured)){
                    break;
                }
                if(m_board[raySq] & (m_side ? WHITE : BLACK)){
                    if((m_board[raySq]&PTMASK) == QUEEN || (m_board[raySq]&PTMASK) == ROOK) return true;
                }
            }
        }
        return false;
    }

    void position::setFEN(std::string fen) {

    }

    void position::perft(int depth, bool divide) {

    }
}
