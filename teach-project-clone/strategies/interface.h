#pragma once

#include "../game/structures.h"

class strategy_interface_t {
public:
    virtual step_t select_piece(PLAYER_SIDE player_side, const field_t &field) = 0;
    virtual step_t select_move(PLAYER_SIDE player_side, const field_t& field, const step_t& current_piece_position) = 0;
    
    virtual void on_incorrect_piece_selection(const step_t& selected_piece_position) = 0;
    virtual void on_nomove_piece_selection(const step_t& selected_piece_position) = 0;
    virtual void on_incorrect_move_selection(
        const step_t& selected_piece_position,
        const step_t& selected_move
    ) = 0;

    virtual void on_lose(PLAYER_SIDE player_side) = 0;
    virtual void on_win(PLAYER_SIDE player_side) = 0;
    virtual void on_tie(PLAYER_SIDE player_side) = 0;
};
