#include <memory>

#include "game/game.h"
#include "strategies/interface.h"
#include "strategies/human.h"
#include "strategies/alpha_computer.h" 
#include "strategies/beta_computer.h"

using computer_t = std::shared_ptr<computer_interface_t>;


void alpha_computer_self_testing(const field_t& field); 
void beta_computer_self_testing(const field_t& field);
void alpha_vs_beta_testing(const field_t& field);
void human_vs_alpha(const field_t& field); 
void human_vs_beta(const field_t& field);
void human_vs_human(const field_t& field);

using namespace std;

int main() {
    field_t field = field_t::create_standard(); 
    
    // OR create your custom field
    // This is the field recreated from the image
    // field_t field = field_t::create_standard(); 
    //  
    // field(0, 1) = EMPTY; 
    // field(0, 3) = EMPTY; 
    // field(1, 0) = EMPTY; 
    // field(1, 2) = EMPTY; 
    // field(1, 4) = EMPTY; 
    // field(2, 7) = EMPTY; 
    // field(3, 6) = BLACK_MAN; 
    //  
    // field(5, 6) = WHITE_KING; 
    // field(4, 7) = WHITE_MAN; 
    // field(5, 0) = EMPTY; 
    // field(6, 1) = EMPTY; 
    // cout << field << endl;  
     
    // alpha_computer_self_testing(field); 
    // beta_computer_self_testing(field); 
    // human_vs_alpha(field);  
    // human_vs_human(field); 
    human_vs_beta(field);

    return 0;
}

void human_vs_human(const field_t& field) { 
    auto player1 = std::make_shared<human_strategy_t>(); 
    auto player2 = std::make_shared<human_strategy_t>(); 
    
    game_t game(player1, player2, field); 
    game.play(); 
} 

void human_vs_alpha(const field_t& field) { 
    auto player1 = std::make_shared<human_strategy_t>(true); 
    auto player2 = std::make_shared<alpha_computer_strategy_t>("Second stupid player", time(0), true); 

    game_t game(player1, player2, field); 
    game.play(); 
} 

void human_vs_beta(const field_t& field) { 
    auto player1 = std::make_shared<human_strategy_t>(true); 
    auto player2 = std::make_shared<beta_computer_strategy_t>("Second smarter player", time(0), true); 

    game_t game(player1, player2, field); 
    game.play(); 
} 

void computer_arena(const computer_t& u, const computer_t& v, const field_t& field, size_t times = 1000) {
    for (int i = 0; i < times; ++i) {
        game_t game(u, v, field);
        game.play();
    }
    u->print_stat();
    v->print_stat();
}


void alpha_computer_self_testing(const field_t& field) { 
    auto player1 = std::make_shared<alpha_computer_strategy_t>("First stupid player", 1u); 
    auto player2 = std::make_shared<alpha_computer_strategy_t>("Second stupid player", 2u); 
    computer_arena(player1, player2, field);
} 

void beta_computer_self_testing(const field_t& field) {
    auto player1 = std::make_shared<alpha_computer_strategy_t>("First smarter player", 1u); 
    auto player2 = std::make_shared<alpha_computer_strategy_t>("Second smarter player", 2u); 
    computer_arena(player1, player2, field);
}

void alpha_vs_beta_testing(const field_t& field) {
    auto player1 = std::make_shared<alpha_computer_strategy_t>("First stupid player", 1u); 
    auto player2 = std::make_shared<alpha_computer_strategy_t>("Second smarter player", 2u); 
    computer_arena(player1, player2, field);
}
