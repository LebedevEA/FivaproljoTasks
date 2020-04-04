#include <vector>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QTimer>
#include "Controller.h"

KeyPresser::KeyPresser(Player *player, QWidget *parent) {
    manipulators_.push_back(new PlayerManipulator(player));
    setWindowOpacity(0.0);
    setFocus();
}

KeyPresser::KeyPresser(Player *player1, Player *player2, QWidget *parent) { // TODO добавить второго игрока в вектор
    unused(player2);
    manipulators_.push_back(new PlayerManipulator(player1));
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

KeyPresser::PlayerManipulator::PlayerManipulator(Player *player, Qt::Key up_key, Qt::Key left_key,
                                                 Qt::Key down_key, Qt::Key right_key)
    : player_(player)
    , UP(up_key)
    , LEFT(left_key)
    , DOWN(down_key)
    , RIGHT(right_key) {
    is_active_ = true; // TODO только пока без меню
}

KeyPresser::Key::Key(Qt::Key qt_name)
    : qt_name_(qt_name)
    , is_pressed_(false)
{}

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
        player_->direction = Utilities::Direction::LEFT;
        player_->change_skin_direction();
    } else if (k == RIGHT) {
        RIGHT.press();
        player_->moving = true;
        player_->direction = Utilities::Direction::RIGHT;
        player_->change_skin_direction();
    }
}

void KeyPresser::PlayerManipulator::release(Qt::Key k) {
        if (k == UP) {
        UP.release();
    } else if (k == LEFT) {
        LEFT.release();
        if (RIGHT.is_pressed()) {
            player_->moving = true;
            player_->direction = Utilities::Direction::RIGHT;
            player_->change_skin_direction();
        } else {
            player_->moving = false;
            player_->direction = Utilities::Direction::UNKNOWN;
        }
    } else if (k == RIGHT) {
        RIGHT.release();
        if (LEFT.is_pressed()) {
            player_->moving = true;
            player_->direction = Utilities::Direction::LEFT;
            player_->change_skin_direction();
        } else {
            player_->moving = false;
            player_->direction = Utilities::Direction::UNKNOWN;
        }
    }
}
