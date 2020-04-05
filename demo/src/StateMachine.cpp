#include "StateMachine.h"

void StateMachine::run(Utilities::GameState game_state) { // Тут разобраться, что и где вызывает activate и deactivate
    if (game_state == Utilities::GameState::MENU) {
    	run_menu();
    } else if (game_state == Utilities::GameState::SINGLE_PLAYER || game_state == Utilities::GameState::MULTI_PLAYER) {
	deactivate(KeyPresserUtility::ManipulatorType::MENU);
    	run_level(game_state);
    }
}

void StateMachine::set_function(std::function<void(Utilities::GameState)> func) {
    run_level = func;
}

void StateMachine::set_function(std::function<void()> func) {
    run_menu = func;
}

void StateMachine::set_activate(std::function<void(KeyPresserUtility::ManipulatorType)> func) {
    activate = func;
}

void StateMachine::set_deactivate(std::function<void(KeyPresserUtility::ManipulatorType)> func) {
    deactivate = func;
}
