#include "position.h"

namespace Chameleon {

std::ostream& operator<< (std::ostream &out, const Position &pos) {
    std::string board {
        " - - - - - - - - - - - -\n"
        " - - - - - - - - - - - -\n"
        " 8 - . . . . . . . . - -\n"
        " 7 - . . . . . . . . - -\n"
        " 6 - . . . . . . . . - -\n"
        " 5 - . . . . . . . . - -\n"
        " 4 - . . . . . . . . - -\n"
        " 3 - . . . . . . . . - -\n"
        " 2 - . . . . . . . . - -\n"
        " 1 - . . . . . . . . - -\n"
        " - - - - - - - - - - - -\n"
        " - - A B C D E F G H - -\n"
    };

    // For each bitboard, replace the given square with the piece
    // representation
    int str_indx  { 55 };
    sq_t sq       { };
    for(int rank { 7 }; rank >= 0; --rank) {
        for(int file { 0 }; file < 8; ++file) {
            sq = sq_t(rank*8 + file);
            if(pos.m_board8x8[sq] != EMPTY) {
                board[str_indx] = PIECE_STR_REP[pos.color(sq)][pos.m_board8x8[sq]];
            }
            if(sq == pos.m_ep) {
                board[str_indx] = 'X';
            }
            str_indx += 2;
        }
        str_indx += 9;
    }

    std::string info { "\n side: " };
    info += pos.m_side ? "white" : "black";
    info += "\tply: ";
    info += std::to_string(pos.m_ply); 
    info += "\n\n";
    info += " FEN: ";
    info += pos.getFEN();

    // Then we finally output that to the stream
    out << board;
    out << info;

    return out;
}


void Position::make(move_t move) {
    // Unpack move info
    sq_t from_sq { from(move) }, to_sq { to(move) };
    mvflag_t fl { flag(move) };

    // Get info about moving pieces and any captured pieces
    const piece_t moving   { m_board8x8[from_sq] },
                  captured { m_board8x8[to_sq] };

    // Start by moving the piece from from_sq to to_sq
    remove(from_sq, moving, m_side);
    add(to_sq, moving, m_side);
    // Remove the captured piece if necessary
    if(captured != EMPTY) remove(to_sq, captured, m_opponent);

    m_ep = inv; // Always invalidate m_ep (since it is reset every move)

    // Then apply flag effects
    if(fl == DPAWNPUSH) m_ep = sq_t(to_sq + (m_side ? -8 : 8));
    else if(fl & NPROM) promote(to_sq, piece_t((fl&0b11) - 1));
    else if(fl & KCASTLE) castle(fl == KCASTLE, m_side);

    if(moving == KING) { // If we move our king, we can't castle anymore
        m_castling &= (m_side ? 0b0011 : 0b1100);
    }
    else if(moving == ROOK) { // Moving a rook disables the ability to castle with it
        m_castling &= (m_side ? (from_sq == h1 ? 0b0111 : 0b1011) 
                              : (from_sq == h8 ? 0b1101 : 0b1110));
    }

    // Switch side to play
    m_side = m_opponent;
    m_opponent = m_side ? BLACK : WHITE;

    // Increment ply, and halfmove clock if not a capture or pawn move
    ++m_ply;
    if(fl & CAP || moving == PAWN) m_halfmove = 0;
    else ++m_halfmove;
}

void Position::unmake() {
    return;
}

void Position::setFEN(const std::string& fen) {
    // Prepare the change by setting bitboards/board to 0
    for(int ptype { PAWN }; ptype < EMPTY; ++ptype) {
        m_ptype_bb[ptype] = EMPTYBB;
    }
    for(int color { BLACK }; color <= BLACK; ++color) {
        m_color_bb[color] = EMPTYBB;
    }
    m_empty_bb = EMPTYBB;
    for(int sq { 0 }; sq < 64; ++sq) {
        m_board8x8[sq] = EMPTY;
    }

    // Split the fen string to get one string/rank
    std::vector<std::string> fen_info { };
    std::string buffer                { };
    for(auto c : fen) {
        if(c == '/' || c == ' ') {
            fen_info.push_back(buffer);
            buffer = "";
        }
        else { buffer += c; }
    }

    // Set boards according to the piece placement
    sq_t curr_sq  { };
    int file      { 0 };
    for(int rank { 7 }; rank >= 0; --rank) {
        file = 0;
        for(auto c : fen_info.at(7-rank)) {
            curr_sq = sq_t(rank*8 + file);
            switch(tolower(c)) {
                case 'p':
                    add(curr_sq, PAWN, color_t(!islower(c)));
                    break;
                case 'n':
                    add(curr_sq, KNIGHT, color_t(!islower(c)));
                    break;
                case 'b':
                    add(curr_sq, BISHOP, color_t(!islower(c)));
                    break;
                case 'r':
                    add(curr_sq, ROOK, color_t(!islower(c)));;
                    break;
                case 'q':
                    add(curr_sq, QUEEN, color_t(!islower(c)));
                    break;
                case 'k':
                    add(curr_sq, KING, color_t(!islower(c)));
                    break;

                default:
                    file += std::stoi(std::string{ c }) - 1;
                    break;
            }
            ++file;
        }
    }

    // Then set the position options
    m_side = color_t(fen_info[8] == "w");
    m_castling = 0;
    for(auto c : fen_info[9]) {
        switch(c){
            case 'K':
                m_castling |= WKCASTLE;
                break;
            case 'k':
                m_castling |= BKCASTLE;
                break;
            case 'Q':
                m_castling |= WQCASTLE;
                break;
            case 'q':
                m_castling |= BQCASTLE;
                break;
            default: break;
        }
    }

    for(int sq { 0 }; sq < 65; ++sq) {
        if(fen_info[10] == SQ_STRING[sq]) {
            m_ep = sq_t(sq); 
            break;
        }
    }

    m_halfmove = std::stoi(fen_info[11]);
    m_ply = std::stoi(fen_info[12]);
}

std::string Position::getFEN() const {
    // Piece placement info
    std::string fen { };
    sq_t curr_sq    { };
    int skipped     { 0 };
    for(int rank { 7 }; rank >= 0; --rank) {
        if(skipped) {
            fen += std::to_string(skipped);
            skipped = 0;
        }
        if(rank != 7) fen += "/";
        for(int file { 0 }; file < 8; file++) {
            curr_sq = sq_t(rank*8 + file);
            if(m_board8x8[curr_sq] != EMPTY && skipped) {
                fen += std::to_string(skipped);
                skipped = 0;
            }
            switch(m_board8x8[curr_sq]) {
                case PAWN:
                    fen += color(curr_sq) ? "P" : "p";
                    break;
                case KNIGHT:
                    fen += color(curr_sq) ? "N" : "n";
                    break;
                case BISHOP:
                    fen += color(curr_sq) ? "B" : "b";
                    break;
                case ROOK:
                    fen += color(curr_sq) ? "R" : "r";
                    break;
                case QUEEN:
                    fen += color(curr_sq) ? "Q" : "q";
                    break;
                case KING:
                    fen += color(curr_sq) ? "K" : "k";
                    break;

                default:
                    ++skipped;
                    break;
            }
        }
    }
    if(skipped) fen += std::to_string(skipped);

    // Then deal with the info of the position
    fen += m_side ? " w " : " b ";
    if(!m_castling) fen += "- ";
    else {
        if(m_castling & WKCASTLE) fen += "K";
        if(m_castling & WQCASTLE) fen += "Q";
        if(m_castling & BKCASTLE) fen += "k";
        if(m_castling & BQCASTLE) fen += "q";
        fen += " ";
    }

    fen += SQ_STRING[m_ep];
    fen += " " + std::to_string(m_halfmove);
    fen += " " + std::to_string(m_ply);


    return fen;
}


/*
* GETTERS
*/

inline color_t Position::color(sq_t sq) const {
    if((ONEBB << sq) & m_color_bb[WHITE]) return WHITE;
    else if((ONEBB << sq) & m_color_bb[BLACK]) return BLACK;
    else return NONE;
}

/*
* SETTERS
*/ 
inline void Position::add(sq_t sq, piece_t piece, color_t color) {
    m_ptype_bb[piece] |= (ONEBB << sq);
    m_color_bb[color] |= (ONEBB << sq);
    m_empty_bb &= ~(ONEBB << sq);
    m_board8x8[sq] = piece;
}

inline void Position::remove(sq_t sq, piece_t piece, color_t color) { 
    m_ptype_bb[piece] &= ~(ONEBB << sq);
    m_color_bb[color] &= ~(ONEBB << sq);
    m_empty_bb |= (ONEBB << sq);
    m_board8x8[sq] = EMPTY; 
}

inline void Position::promote(sq_t sq, piece_t target_p) {
    m_ptype_bb[PAWN] &= ~(ONEBB << sq);
    m_ptype_bb[target_p] |= (ONEBB << sq);
    m_board8x8[sq] = target_p;
}

inline void Position::castle(bool kingside, color_t color) {
    sq_t target { sq_t(kingside ? f1 + color * 52 : d1 + color * 52) }, 
         origin { sq_t(kingside ? h1 + color * 52 : a1 + color * 52) };
    m_ptype_bb[ROOK] ^= (ONEBB << target | ONEBB << origin);
    m_color_bb[color] ^= (ONEBB << target | ONEBB << origin);
    m_empty_bb ^= (ONEBB << target | ONEBB << origin);
    m_board8x8[origin] = EMPTY;
    m_board8x8[target] = ROOK;
}

}