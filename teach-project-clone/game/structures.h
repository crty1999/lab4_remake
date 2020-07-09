#pragma once

#include <string>
#include <iostream>
#include <unordered_set>

enum CELL {
    BLACK_MAN = 'b',
    WHITE_MAN = 'w',
    BLACK_KING = 'B',
    WHITE_KING = 'W',
    EMPTY = '.',
    CAPTURED = 'x',
    FORBIDDEN = ' '
};

CELL crowding(CELL cell);
bool is_king(CELL cell);

enum PLAYER_SIDE {
    WHITE = 0,
    BLACK = 1,
    NONE
};

PLAYER_SIDE player_side_of_cell(CELL cell);
PLAYER_SIDE inverse_side(PLAYER_SIDE player_side);
std::string player_side_to_string(PLAYER_SIDE side);

struct step_t {
    // int x, y; 
    // step_t(int x, int y); 
    
    int r, c;
    step_t() = default;
    step_t(int r_, int c_);
    
    step_t(const std::string& notation);
    
    step_t translate(int dr, int dc) const;
    step_t& add(int dr, int dc);
    
    std::string to_notation() const;
    
    friend bool operator==(const step_t& lhs, const step_t& rhs);
    friend bool operator!=(const step_t& lhs, const step_t& rhs);
};

namespace std {
    template<> struct hash<step_t> {
        size_t operator()(const step_t& s) const {
            return hash<long long>()(1ll * s.r << 32LL | s.c);
        }
    };
}

struct move_set {
    step_t seleted_piece;
    std::unordered_set<step_t> normal_moves, jump_moves;
    
    move_set();
    
    bool is_empty() const;
    bool contains(const step_t& step) const;
    
    std::unordered_set<step_t> to_one_set() const;
};

struct field_t {
    static const int NUM_ROW = 8;
    const static int NUM_COL = 8;
    
    CELL fld[NUM_ROW][NUM_COL];
    
    field_t();
    field_t(const field_t& other);
    
    static field_t create_standard();
    
    CELL& operator()(int r, int c);
    CELL& operator[](const step_t& step);
    
    CELL operator()(int r, int c) const;
    CELL operator[](const step_t& step) const;
    
    friend std::ostream& operator<<(std::ostream& out, const field_t& field);
    
    move_set compute_move_set(const step_t& selected_piece) const;
    
    void clear_captured();
};
