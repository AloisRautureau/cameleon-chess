//
// Created by bitterboyy on 4/16/21.
//

#ifndef CHAMELEON_POSITION_H
#define CHAMELEON_POSITION_H

#include <string>
#include "defs.h"

namespace Chameleon {
        class position {
        protected:
            piece m_board[0x88]{EMPTY}; //The board holding piece placement information
            plist m_plists[2][6]; //Piece position info, with an array of 2x6 (color x pieces)

            bool m_side = 0; //Variable to keep track of side to move with 0 = white and 1 = black
            uint8_t m_castling = 0b1111; //Variable keeping track of castling rights (KQkq)
            int m_ep = 0x88; //Variable keeping track of en passant square (0x88 means no en passant square is available)
            int m_ply = 0; //Number of moves played
            int m_fifty = 0; //Keeps track of fifty move rule
            bool m_checked = false; //Set to true if king checked

            pins m_pinned; //Pinned pieces
            pins m_pinning; //Pinning pieces

        public:
            position();

            /*
            * GENERATORS
            */
            //Generates all legal moves for the given position
            void gen(movestack &stack);

            //Generates non quiet moves (captures, promotions, checks)
            void genNoisy(movestack &stack);

            //Generates moves to get the king out of check
            void genEvasion(movestack &stack);

            /*
            * MAKE/UNMAKE
            */
            //Makes the move on the board
            void make(movebyte move);

            //Unmakes the last move played
            void unmake();

            /*
            * UTILITY
            */
            //Returns true if the given square is attacked by the opponent.
            //The kingxray flag dictates whether or not side-to-move's king should be ignored (useful to check for legal king moves)
            bool isAttacked(int square, bool kingxray = false);

            //Calculates pinned pieces and stores them in an appropriate container (square of the pinned piece + delta it can move on)
            void updatePins();

            //Checks for a rare case in which en passant moves can get illegal when the attacking pawn isn't pinned
            bool epGotcha(int epSquare, int captured);

            //Sets the board state to the given FEN string
            void setFEN(std::string fen);

            //Used to debug the position class, by making and unmaking all possible moves and comparing the results to
            //correct move generators
            void perft(int depth, bool divide);
        };
}
#endif //CHAMELEON_POSITION_H
