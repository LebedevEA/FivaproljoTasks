#include <vector>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QTimer>
#include "Controller.h"

void Controller::runGame() { // TODO в меню чистить поле
    players_.push_back(new Player());
    model_ = new Model();
    model_->add_players(players_);
    scene_ = new Scene();
    key_presser_ = new KeyPresser(players_[0]);
    key_presser_->setFixedSize(QSize(scene_->scene()->width(), scene_->scene()->height()));
    scene_->addWidget(key_presser_);
    menu_ = new Menu(scene_);
    
   
    
    // if (gamemod == RUN_SINGLEPLAYER) {
    // 	model_->make_new_level(scene_);
    // 	// TODO я не знаю где показывает статистику
    // 	call_menu();
    // } else if (gamemod == EXIT) {
    // 	// TODO???????? видимо ничего ???????
    // }
}

void KeyPresser::helper() {
    if (menu_->is_active && menu_->state != MenuRetVal::NONE) {
	menu_->is_active = false;
	menu_->state == MenuRetVal::NONE;
	model_->make_new_level(scene_); // activate
	for (auto manip : manipulators_) {
	    manip->deactivate();
	}
	manipulators_[0]->activate();
    } else if (model_->active() && model_->finished()) {
	model_->deactivate();
	menu_->run_menu();
	for (auto manip : manipulators_) {
	    manip->deactivate();
	}
	manipulators_[1]->activate(); // TODO сделать нормально а не обращаться к какому-то номеру + его может не быть
    }
}

KeyPresser::KeyPresser(Player *player, QWidget *parent) {
    manipulators_.push_back(new PlayerManipulator(player));
    manipulators_.push_back(new MenuManipulator);
    setWindowOpacity(0.0);
    setFocus();
}

KeyPresser::KeyPresser(Player *player1, Player *player2, QWidget *parent) { // TODO добавить второго игрока в вектор
    unused(player2);
    manipulators_.push_back(new PlayerManipulator(player1));
    manipulators_.push_back(new MenuManipulator);  
    setWindowOpacity(0.0);
    setFocus();
}

void KeyPresser::keyPressEvent(QKeyEvent *event) {
    for (auto manip : manipulators_) {
	if (manip->active()) {
            manip->press((Qt::Key)event->key()); //TODO cast
	}
    }
}

void KeyPresser::keyReleaseEvent(QKeyEvent *event) {
    for (auto manip : manipulators_) {
	if (manip->active()) {
            manip->release((Qt::Key)event->key()); //TODO cast
	}
    }
}

bool KeyPresser::Manipulator::active() const {
    return is_active_;
}

void KeyPresser::Manipulator::activate() {
    is_active_ = true;
}

void KeyPresser::Manipulator::deactivate() {
    is_active_ = false;
}

KeyPresser::MenuManipulator::MenuManipulator()
    : UP(Qt::Key_W)
    , DOWN(Qt::Key_S)
{}

void KeyPresser::MenuManipulator::press(Qt::Key k) {
    if (k == UP) {
	menu_->go_up();
    } else if (k == DOWN) {
	menu_->go_down();
    }
}

void KeyPresser::MenuManipulator::release(Qt::Key k) {}

KeyPresser::PlayerManipulator::PlayerManipulator(Player *player, Qt::Key up_key, Qt::Key left_key,
                                                 Qt::Key down_key, Qt::Key right_key)
    : player_(player)
    , UP(up_key)
    , LEFT(left_key)
    , DOWN(down_key)
    , RIGHT(right_key) {
    is_active_ = true; // TODO только пока без меню
}

KeyPresser::Key::Key(Qt::Key qt_name) : qt_name_(qt_name) {}

KeyPresser::Key::operator Qt::Key() const {
    return qt_name_;
}

bool KeyPresser::Key::is_pressed() const {
    return is_pressed_;
}

void KeyPresser::Key::press() {
    is_pressed_ = true;
}

void KeyPresser::Key::release() {
    is_pressed_ = false;
}

void KeyPresser::PlayerManipulator::press(Qt::Key k) {
    if (k == UP) {
        UP.press();
        player_->start_jumping();
    } else if (k == LEFT) {
        LEFT.press();
        player_->moving = true;
        player_->direction = Direction::LEFT;
        player_->change_direction();
    } else if (k == RIGHT) {
        RIGHT.press();
        player_->moving = true;
        player_->direction = Direction::RIGHT;
        player_->change_direction();
    }
}

void KeyPresser::PlayerManipulator::release(Qt::Key k) {
        if (k == UP) {
        UP.release();
    } else if (k == LEFT) {
        LEFT.release();
        if (RIGHT.is_pressed()) {
            player_->moving = true;
            player_->direction = Direction::RIGHT;
            player_->change_direction();
        } else {
            player_->moving = false;
            player_->direction = Direction::UNKNOWN;
        }
    } else if (k == RIGHT) {
        RIGHT.release();
        if (LEFT.is_pressed()) {
            player_->moving = true;
            player_->direction = Direction::LEFT; // Left
            player_->change_direction();
        } else {
            player_->moving = false;
            player_->direction = Direction::UNKNOWN;
        }
    }
}
