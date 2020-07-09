#pragma once

#include <memory>
#include <vector>

#include "../strategies/interface.h"
#include "structures.h"

class game_t {
public:
    using player_t = std::shared_ptr<strategy_interface_t>;
    game_t(
            const player_t &first,
            const player_t &second,
            const field_t& init_field = field_t::create_standard()
        );

    void play();

private:
    enum game_state_t {
        ENDED,
        TIE,
        IN_PROGRESS
    };
    
    bool player_has_move(PLAYER_SIDE player_num) const;
    
    game_state_t get_game_state(PLAYER_SIDE player_num) const;
    bool partially_apply_step(const step_t& selected_piece, const step_t& selected_move, PLAYER_SIDE player_num);
    void end_moves(PLAYER_SIDE player_num);

    field_t field;
    std::vector<player_t> players;
};

