#include "alpha_computer.h"
#include "../utils.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

alpha_computer_strategy_t::alpha_computer_strategy_t(const string& name, size_t seed, bool print_moves_)
    : computer_interface_t(name, print_moves_)
    , rng(seed)
{}

step_t alpha_computer_strategy_t::select_piece(PLAYER_SIDE player_side, const field_t &field) {
    vector<step_t> possible_pieces;
    for (int r = 0; r < field_t::NUM_ROW; ++r) {
        for (int c = 0; c < field_t::NUM_COL; ++c) {
            step_t step(r, c);
            if (player_side_of_cell(field[step]) == player_side) {
                possible_pieces.push_back(step);
            }
        }
    }
    
    step_t res = * choice(possible_pieces.begin(), possible_pieces.end(), rng);
    
    if (print_moves) {
        cout << field << endl;
        cout << name << " chooses " << res.to_notation() << endl;
    }
    return res;
}

step_t alpha_computer_strategy_t::select_move(PLAYER_SIDE player_side, const field_t& field, const step_t& current_piece_position) {
    auto moves = field.compute_move_set(current_piece_position).to_one_set();
    assert(moves.size());
    
    step_t res = *choice(moves.begin(), moves.end(), rng);
    if (print_moves) {
        cout << field << endl;
        cout << name << " tries to move " << current_piece_position.to_notation() << " to " << res.to_notation() << endl;
    }
    return res;
}

