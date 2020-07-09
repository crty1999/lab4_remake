#pragma once

#include <string>
#include "interface.h"

class computer_interface_t : public strategy_interface_t {
public:
    computer_interface_t(const std::string& name_, bool print_moves = false);

    void on_win(PLAYER_SIDE player_side) override;
    void on_lose(PLAYER_SIDE player_side) override;
    void on_tie(PLAYER_SIDE player_side) override;
  
    virtual void on_incorrect_piece_selection(const step_t& selected_piece_position) override;
    virtual void on_nomove_piece_selection(const step_t& selected_piece_position) override;
    virtual void on_incorrect_move_selection(
        const step_t& selected_piece_position,
        const step_t& selected_move
    ) override;

    virtual void print_stat() const;

protected:
    std::string name;
    bool print_moves;
    
private:
    size_t win_count;
    size_t lose_count;
    size_t tie_count;
    size_t nomove_piece_selection_count;
    size_t incorrect_piece_selection_count;
    size_t incorrect_move_selection_count;
};
