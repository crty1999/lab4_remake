#include "game.h"
#include <utility>
#include <cassert>
#include "../utils.h"

using namespace std;

game_t::game_t(
        const player_t &first,
        const player_t &second,
        const field_t& init_field
) : field(init_field)
{
    players.push_back(first);
    players.push_back(second);
}

void game_t::play() {
    PLAYER_SIDE player_num = PLAYER_SIDE::WHITE;
    for (; get_game_state(player_num) == IN_PROGRESS;
            player_num = inverse_side(player_num) 
    ) {
        step_t selected_piece, selected_move(-1, -1);
        move_set current_move_set;
        
        auto& current_player = players[player_num];
        
        while (true) {
            selected_piece = current_player->select_piece(player_num, field);
            if (player_side_of_cell(field[selected_piece]) != player_num) {
                current_player->on_incorrect_piece_selection(selected_piece);
                continue;
            }
            current_move_set = field.compute_move_set(selected_piece);
            if (current_move_set.is_empty()) {
                current_player->on_nomove_piece_selection(selected_piece);
                continue;
            }
            break;
        }
        
        while (true) {
            selected_move = current_player->select_move(player_num, field, selected_piece);
            
            if (!current_move_set.contains(selected_move)) {
                current_player->on_incorrect_move_selection(selected_piece, selected_move);
            } else {
                break;
            }
        }
        
        bool ok = partially_apply_step(selected_piece, selected_move, player_num);
        assert(ok);  // it must be ok since we have already checked with our move set
        
        if (current_move_set.jump_moves.count(selected_move)) {
            while (true) {
                selected_piece = selected_move;
                current_move_set = field.compute_move_set(selected_piece);
                if (current_move_set.jump_moves.size() == 0) {
                    break;
                }
                while (true) {
                    selected_move = current_player->select_move(player_num, field, selected_piece);
                    if (!current_move_set.jump_moves.count(selected_move)) {
                        current_player->on_incorrect_move_selection(selected_piece, selected_move);
                    } else {
                        break;
                    }
                }
                bool ok = partially_apply_step(selected_piece, selected_move, player_num);
                assert(ok);
            }
        }
        
        end_moves(player_num);
    }

    switch (get_game_state(player_num)) {
        case TIE: {
            players[0]->on_tie(WHITE);
            players[1]->on_tie(BLACK);
            break;
        }
                  
        case ENDED: {
            players[!player_num]->on_win(inverse_side(player_num));
            players[player_num]->on_lose(player_num);
            break;
        }
    
        default: assert(false);
    }
}


bool game_t::player_has_move(PLAYER_SIDE player_num) const {
    for (int r = 0; r < field_t::NUM_ROW; ++r) {
        for (int c = 0; c < field_t::NUM_COL; ++c) {
            step_t st(r, c);
            if (player_side_of_cell(field(r, c)) != player_num) {
                continue;
            }
            if (!field.compute_move_set(st).is_empty()) {
                return true;
            }
        }
    }
    return false;
}
    
game_t::game_state_t game_t::get_game_state(PLAYER_SIDE player_num) const {
    assert(player_num != PLAYER_SIDE::NONE);
    
    if (player_has_move(player_num)) {
        return game_state_t::IN_PROGRESS;
    }
    
    if (!player_has_move(inverse_side(player_num))) {
        return game_state_t::TIE;
    }
    
    return game_state_t::ENDED;
}

bool game_t::partially_apply_step(const step_t& selected_piece, const step_t& selected_move, PLAYER_SIDE player_num) {
    int dr = selected_piece.r < selected_move.r ? 1 : -1;
    int dc = selected_piece.c < selected_move.c ? 1 : -1;
    
    CELL old_piece = field[selected_piece];
    field[selected_piece] = CELL::EMPTY;
    
    for (auto it = selected_piece; it != selected_move; it.add(dr, dc)) {
        if (field[it] == CELL::EMPTY) {
            continue;
        }
        if (player_side_of_cell(field[it]) == player_num) {
            return false;
        }
        field[it] = CELL::CAPTURED;
    }
    
    field[selected_move] = old_piece;
    if ((player_num == WHITE && selected_move.r == 0 && !is_king(old_piece)) || 
        (player_num == BLACK && selected_move.r == field_t::NUM_ROW - 1 && !is_king(old_piece))
    ) {
        field[selected_move] = crowding(old_piece);
    }
    
    return true;
}

void game_t::end_moves(PLAYER_SIDE player_num) {
    field.clear_captured();
}
