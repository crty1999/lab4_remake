#include "structures.h"
#include <cstring>
#include <cassert>

using namespace std;

CELL crowding(CELL cell) {
    if (cell == BLACK_MAN) {
        return BLACK_KING;
    }
    if (cell == WHITE_MAN) {
        return WHITE_KING;
    }
    return FORBIDDEN;
}

bool is_king(CELL cell) {
    return cell == WHITE_KING || cell == BLACK_KING;
}

PLAYER_SIDE player_side_of_cell(CELL cell) {
    switch (cell) {
        case CELL::WHITE_MAN:
        case CELL:: WHITE_KING:
            return PLAYER_SIDE::WHITE;
        case CELL::BLACK_MAN:
        case CELL::BLACK_KING:
            return PLAYER_SIDE::BLACK;
        default:
            return PLAYER_SIDE::NONE;
    }
}

PLAYER_SIDE inverse_side(PLAYER_SIDE player_side) {
    if (player_side == WHITE) return BLACK;
    if (player_side == BLACK) return WHITE;
    return NONE;
}

std::string player_side_to_string(PLAYER_SIDE side) {
    switch (side) {
        case WHITE: return "WHITE";
        case BLACK: return "BLACK";
        case NONE: return "NONE";
    }
    return "";
}


// step_t
//////////////////////
step_t::step_t(int r_, int c_) : r(r_), c(c_) {}

step_t::step_t(const string& notation) {
    assert(notation.size() == 2);
    assert('A' <= notation[0] and notation[0] <= 'H');
    assert('1' <= notation[1] and notation[1] <= '8');
    r = notation[1] - '1';
    c = notation[0] - 'A';
}

std::string step_t::to_notation() const {
    return string() + char(c + 'A') + char(r + '1');
}

step_t step_t::translate(int dr, int dc) const {
    return step_t(r + dr, c + dc);
}

step_t& step_t::add(int dr, int dc) {
    r += dr;
    c += dc;
    return *this;
}

bool operator==(const step_t& lhs, const step_t& rhs) {
    return lhs.r == rhs.r && lhs.c == rhs.c;
}

bool operator!=(const step_t& lhs, const step_t& rhs) {
    return !(lhs == rhs);
}

// move_set
/////////////////////////
move_set::move_set() {}

bool move_set::is_empty() const {
    return normal_moves.empty() && jump_moves.empty();
}

bool move_set::contains(const step_t& step) const {
    return normal_moves.count(step) || jump_moves.count(step);
}

unordered_set<step_t> move_set::to_one_set() const {
    unordered_set<step_t> res = normal_moves;
    res.insert(jump_moves.begin(), jump_moves.end());
    return res;
}
    

// field_t
/////////////////////////
field_t::field_t() {
    for (int r = 0; r < field_t::NUM_ROW; ++r) {
        for (int c = 0; c < field_t::NUM_COL; ++c) {
            fld[r][c] = (r + c) % 2 == 0 ? CELL::FORBIDDEN : CELL::EMPTY;
        }
    }
}

field_t::field_t(const field_t& other) {
    memcpy(fld, other.fld, sizeof(fld));
}

field_t field_t::create_standard() {
    field_t res;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < NUM_COL; ++c) {
            if (res.fld[r][c] == CELL::EMPTY) {
                res.fld[r][c] = CELL::BLACK_MAN;
            }
        }
    }
    
    for (int r = NUM_ROW - 1; r >= NUM_ROW - 3; --r) {
        for (int c = 0; c < NUM_COL; ++c) {
            if (res.fld[r][c] == CELL::EMPTY) {
                res.fld[r][c] = CELL::WHITE_MAN;
            }
        }
    }
    return res;
}

CELL& field_t::operator()(int r, int c) {
    if (r < 0 or r > NUM_ROW or c < 0 or c > NUM_COL) {
        assert(false);
    }
    return fld[r][c];
}

CELL& field_t::operator[](const step_t& step) {
    return (*this)(step.r, step.c);
}

CELL field_t::operator()(int r, int c) const {
    if (r < 0 or r > NUM_ROW or c < 0 or c > NUM_COL) {
        return CELL::FORBIDDEN;
    }
    return fld[r][c];
}

CELL field_t::operator[](const step_t& step) const {
    return (*this)(step.r, step.c);
}

move_set field_t::compute_move_set(const step_t& step) const {
    move_set res;
    CELL cur_cell = (*this)[step];
    PLAYER_SIDE cur_side = player_side_of_cell(cur_cell);
    if (cur_side == PLAYER_SIDE::NONE) {
        return res;
    }
    
    if (is_king(cur_cell)) {
        for (int dr: {-1, 1}) {
            for (int dc: {-1, 1}) {
                auto new_step = step.translate(dr, dc);
                CELL new_cell = (*this)[new_step];
                for (; new_cell == CELL::EMPTY; new_cell = (*this)[new_step.add(dr, dc)]) { 
                    res.normal_moves.insert(new_step);
                }
                PLAYER_SIDE new_cell_side = player_side_of_cell(new_cell);
                if (new_cell_side == cur_side or new_cell_side == PLAYER_SIDE::NONE) {
                    continue;
                }
                for (new_step.add(dr, dc); (*this)[new_step] == CELL::EMPTY; new_step.add(dr, dc)) {
                    res.jump_moves.insert(new_step);
                }
            }
        }
    } else {
        const int forward_r = cur_side == WHITE ? -1 : 1;
        for (int dc: {-1, 1}) {
            {
                auto temp_step = step.translate(forward_r, dc);
                if ((*this)[temp_step] == CELL::EMPTY) {
                    res.normal_moves.insert(temp_step);
                }
            }
            
            for (int dr: {-1, 1}) {
                auto step1 = step.translate(dr, dc);
                PLAYER_SIDE step1_side = player_side_of_cell((*this)[step1]);
                if (step1_side == cur_side or step1_side == PLAYER_SIDE::NONE) {
                    continue;
                }
                if ((*this)[step1.add(dr, dc)] == CELL::EMPTY) {
                    res.jump_moves.insert(step1);
                }
            }
        }
    }
    
    return res;
}

void field_t::clear_captured() {
    for (int r = 0; r < field_t::NUM_ROW; ++r) {
        for (int c = 0; c < field_t::NUM_COL; ++c) {
            if (fld[r][c] == CELL::CAPTURED) {
                fld[r][c] = CELL::EMPTY;
            }
        }
    }
}

std::ostream& operator<<(std::ostream& out, const field_t& field) {
    out << " |A|B|C|D|E|F|G|H|" << endl;
    for (int r = 0; r < field_t::NUM_ROW; ++r) {
        out << (r + 1) << "|";
        for (int c = 0; c < field_t::NUM_COL; ++c) {
            out << (char)field(r, c) << '|';
        }
        out << endl;
    }
    return out;
}

