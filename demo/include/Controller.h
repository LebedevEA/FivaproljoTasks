#pragma once

#include <vector>
#include <functional>
#include <QWidget>
#include <QtGui>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsItem>
#include "Player.h"
#include "Scene.h"
#include "Model.h"
#include "KeyPresser.h"
#include "Menu.h"

class Controller {
public:
    Controller();
    void runGame(); // later - change name to start/begin
    void run_level(Utilities::GameState game_state);
    std::function<void(Utilities::GameState)> get_run_level();
    
private:
    std::vector<Player *> players_;
    Model *model_ = nullptr;
    Scene *scene_ = nullptr;
    KeyPresser *key_presser_ = nullptr;
    Menu *menu_ = nullptr;
    StateMachine *state_machine_ = nullptr;
    MenuHelper *menu_helper_ = nullptr; // Do not use, exists to delete it in destructor
    KeyPresserHelper *key_presser_helper_ = nullptr;
};
