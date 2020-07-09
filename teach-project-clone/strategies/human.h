#pragma once

#include "interface.h"

class human_strategy_t : public strategy_interface_t {
public:
    human_strategy_t(bool print_possible_moves_ = false);
  
    virtual step_t select_piece(PLAYER_SIDE player_side, const field_t &field) override;
    virtual step_t select_move(PLAYER_SIDE player_side, const field_t& field, const step_t& current_piece_position) override;
    
    virtual void on_incorrect_piece_selection(const step_t& selected_piece_position) override;
    virtual void on_nomove_piece_selection(const step_t& selected_piece_position) override;
    virtual void on_incorrect_move_selection(
        const step_t& selected_piece_position,
        const step_t& selected_move
    ) override;

    virtual void on_lose(PLAYER_SIDE player_side) override;
    virtual void on_win(PLAYER_SIDE player_side) override;
    virtual void on_tie(PLAYER_SIDE player_side) override;
    
private:
    bool print_possible_moves;
};
