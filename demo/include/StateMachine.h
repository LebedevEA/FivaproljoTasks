#pragma once

#include <functional>
#include "Utilities.h"
#include "Scene.h"

class StateMachine {
public: // TODO naming of setters
    void run(Utilities::GameState game_state);
    void set_function(std::function<void(Utilities::GameState)> func);
    void set_function(std::function<void()> func);
    void set_activate(std::function<void(KeyPresserUtility::ManipulatorType)> func);
    void set_deactivate(std::function<void(KeyPresserUtility::ManipulatorType)> func);
    
private:
    std::function<void(Utilities::GameState)> run_level = nullptr; // from Controller
    std::function<void()> run_menu = nullptr; // from Menu
    std::function<void(KeyPresserUtility::ManipulatorType)> activate = nullptr; // from KeyPresser
    std::function<void(KeyPresserUtility::ManipulatorType)> deactivate = nullptr; // from KeyPresser
};
