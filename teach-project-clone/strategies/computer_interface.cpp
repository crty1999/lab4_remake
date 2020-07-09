#include "computer_interface.h"

#include <cassert>
#include <iostream>

using namespace std;

computer_interface_t::computer_interface_t(const string& name_, bool print_moves_)
    : name(name_)
    , print_moves(print_moves_)
      
    , win_count{0}
    , lose_count{0}
    , tie_count{0}
    , nomove_piece_selection_count(0)
    , incorrect_piece_selection_count(0)
    , incorrect_move_selection_count(0)
{}

void computer_interface_t::on_win(PLAYER_SIDE player_side) {
  ++win_count;
}
void computer_interface_t::on_lose(PLAYER_SIDE player_side) {
  ++lose_count;
}
void computer_interface_t::on_tie(PLAYER_SIDE player_side) {
  ++tie_count;
}

void computer_interface_t::on_incorrect_piece_selection(const step_t& selected_piece_position) {
    ++incorrect_piece_selection_count;
    if (print_moves) {
        cout << name << " has selected cell without its piece " <<  selected_piece_position.to_notation() << endl;
    }
}
void computer_interface_t::on_nomove_piece_selection(const step_t& selected_piece_position) {
    ++nomove_piece_selection_count;
    if (print_moves) {
        cout << name << " has selected piece without any valid moves " << selected_piece_position.to_notation() << endl;
    }
}
void computer_interface_t::on_incorrect_move_selection(
    const step_t& selected_piece_position,
    const step_t& selected_move
) {
    ++incorrect_move_selection_count;
    if (print_moves) {
        cout << name  << " has selected a wrong move from " << selected_piece_position.to_notation() << " to " << selected_move.to_notation() << endl;
    }
}

void computer_interface_t::print_stat() const {
    cout << "Computer strategy " << name << endl;
    cout << "\twin count: " << win_count << endl;
    cout << "\tlose count: " << lose_count << endl;
    cout << "\ttie count: " << tie_count << endl;
    cout << "\tnomove piece selection count: " << nomove_piece_selection_count << endl;
    cout << "\tincorrect piece selection count: " << incorrect_piece_selection_count << endl;
    cout << "\tincorrect move selection count: " << incorrect_move_selection_count << endl;
}
