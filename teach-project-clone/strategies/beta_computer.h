#pragma once

#include <string>
#include <random>

#include "../game/structures.h"
#include "computer_interface.h"

// 80% of the time will try to pick piece with jump moves (if any)
// 80% of the time will try to pick the jump moves that lead to another jump moves
class beta_computer_strategy_t : public computer_interface_t {
public:
    beta_computer_strategy_t(const std::string& name, size_t random_seed, bool print_moves_ = false);

    virtual step_t select_piece(PLAYER_SIDE plaer_side, const field_t &field) override;
    virtual step_t select_move(PLAYER_SIDE player_side, const field_t& field, const step_t& current_piece_position) override;
    
private:
    std::mt19937 rng;
};
