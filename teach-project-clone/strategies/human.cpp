#include "human.h"
#include "../utils.h"

#include <iostream>
#include <cassert>
#include <string>
using namespace std;

human_strategy_t::human_strategy_t(bool print_possible_moves_)
    : print_possible_moves(print_possible_moves_) {}

    
void blaming_user_for_entering_more_input(const string& accepted_input, const string& remaining_input) {
    // seriously though, you should answer only 1 question at a time.
    cout << "> Got input " << accepted_input << "." << endl;
    cout << "> Seem like you have enter more than 1 word." << endl;
    cout << "> You were SUPPOSED to enter ONLY 1 word." << endl;
    cout << "> Why? Because there might be a misktake in your input." << endl;
    cout << "> Please enter \"" << remaining_input << "\" in the next line if you meant to" << endl;
}

step_t human_strategy_t::select_piece(PLAYER_SIDE player_side, const field_t &field) {
    assert(player_side != PLAYER_SIDE::NONE);
    cout << field << endl;
    cout << "Your side: " << player_side_to_string(player_side) << endl;
    cout << "Select your piece: ";
    string pos, temp;
    auto ss = stdin_line_stream();
    ss >> pos;
    if (ss >> temp) {
        blaming_user_for_entering_more_input(pos, temp);
    }
    return step_t(pos);
}

step_t human_strategy_t::select_move(PLAYER_SIDE player_side, const field_t& field, const step_t& current_piece_position) {
    assert(player_side != PLAYER_SIDE::NONE);
    cout << "Your side: " << player_side_to_string(player_side) << endl;
    cout << field << endl;
    cout << "Your selected piece " << current_piece_position.to_notation() << endl;
    if (print_possible_moves) {
        auto moves = field.compute_move_set(current_piece_position);
        cout << "- Normal moves" << endl << "\t";
        for (auto i: moves.normal_moves) cout << i.to_notation() << ' ';
        cout << endl;
        
        cout << "- Jump moves" << endl << "\t";
        for (auto i: moves.jump_moves) cout << i.to_notation() << ' ';
        cout << endl;
    }
    
    cout << "Where to move your piece?: ";
    string pos, temp;
    auto ss = stdin_line_stream();
    ss >> pos;
    if (ss >> temp) {
        blaming_user_for_entering_more_input(pos, temp);
    }
    return step_t(pos);
}

void human_strategy_t::on_incorrect_piece_selection(const step_t& selected_piece_position) {
    cout << "Cell " << selected_piece_position.to_notation() << " does not contain your piece!" << endl;
}
void human_strategy_t::on_nomove_piece_selection(const step_t& selected_piece_position) {
    cout << "Cell " << selected_piece_position.to_notation() << " contains your piece but has no move!" << endl;
}

void human_strategy_t::on_incorrect_move_selection(
    const step_t& selected_piece_position,
    const step_t& selected_move
) {
    cout << "Cannot move your piece from " << selected_piece_position.to_notation()
        << " to " << selected_move.to_notation() << "!" << endl;
}

void human_strategy_t::on_win(PLAYER_SIDE player_side) {
  std::cout << player_side_to_string(player_side) << ", you win!" << std::endl;
}
void human_strategy_t::on_lose(PLAYER_SIDE player_side) {
  std::cout << player_side_to_string(player_side) << ", you lose!" << std::endl;
}
void human_strategy_t::on_tie(PLAYER_SIDE player_side) {
  std::cout << " Tie!" << std::endl;
}

