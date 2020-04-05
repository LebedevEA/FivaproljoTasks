#include <vector>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QTimer>
#include "Controller.h"

Controller::Controller() { // TODO мембер инициализер лист
    model_ = new Model();
    scene_ = new Scene();
    key_presser_ = new KeyPresser();
    state_machine_ = new StateMachine();
    menu_ = new Menu(scene_, state_machine_);
    menu_helper_ = new MenuHelper(menu_);
    key_presser_helper_ = new KeyPresserHelper(key_presser_);
    key_presser_->add_menu(menu_);
    
    state_machine_->set_function(this->get_run_level());
    state_machine_->set_function(menu_helper_->get_run_menu());
    state_machine_->set_activate(key_presser_helper_->get_activate());
    state_machine_->set_deactivate(key_presser_helper_->get_deactivate());
    
    key_presser_->setFixedSize(QSize(scene_->scene()->width(), scene_->scene()->height()));
    scene_->addWidget(key_presser_);
}

void Controller::runGame() {
    state_machine_->run(Utilities::GameState::MENU);
}

std::function<void(Utilities::GameState)> Controller::get_run_level() {
    return std::bind(&Controller::run_level, *this, std::placeholders::_1);
}

void Controller::run_level(Utilities::GameState game_state) {
    if (game_state == Utilities::GameState::MULTI_PLAYER) {
	players_.push_back(new Player(Utilities::Color::GREEN, "images/demo_player.png"));
	players_.push_back(new Player(Utilities::Color::YELLOW, "images/demo_player.png"));
	key_presser_->add_players(players_[0], players_[1]);
	model_->add_players(players_);
	model_->set_statistics();
	model_->make_new_level(scene_);
	/* TODO когда закончился уровень удалить всех игроков и все из модели
         * И удалить все манипуляторы игроков из KeyPresser (отдельная функция)
	 * Не сейчас, ибо игра бесконечная, но к запуску меню после этого все готово
	 */ 
    }
}
