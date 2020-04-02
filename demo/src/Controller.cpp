#include <vector>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QTimer>
#include "Controller.h"

void Controller::runGame() { // later - add loop
    players_.push_back(new Player());
    model_ = new Model();
    model_->add_players(players_);
    scene_ = new Scene();
    key_presser_ = new KeyPresser(players_[0]);
    key_presser_->setFixedSize(QSize(scene_->scene()->width(), scene_->scene()->height()));
    scene_->addWidget(key_presser_);
    model_->make_new_level(scene_);
}

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
    // if (!event->isAutoRepeat()) {
    // 	if (player_manipulator_.is_active()) {
    // 	    player_manipulator_.press((Qt::Key) event->key()); //TODO cast
    // 	    qDebug() << "Pressed!";
    // 	}
    // 	if (second_player_manipulator.is_active()) {
    // 	    second_player_manipulator.press((Qt::Key) event->key()); //TODO cast
    // 	    qDebug() << "Pressed!";
    // 	}
    // }
}

void KeyPresser::keyReleaseEvent(QKeyEvent *event) {
    for (auto manip : manipulators_) {
	if (manip->active()) {
            manip->release((Qt::Key)event->key()); //TODO cast
	}
    }
    // if (player_manipulator_.is_active()) {
    // 	player_manipulator_.release((Qt::Key) event->key()); //TODO cast
    // }
    // if (second_player_manipulator.is_active()) {
    // 	second_player_manipulator.release((Qt::Key) event->key()); //TODO cast
    // }
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

// KeyPresser::PlayerManipulator_::PlayerManipulator_() // TODO костылина для Насти (смотри хедер строка 24)
//     : player_(nullptr), UP(Qt::Key_W), RIGHT(Qt::Key_D), LEFT(Qt::Key_A), DOWN(Qt::Key_S)
// {}

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
        player_->direction = Direction::LEFT; // Left
        player_->change_direction();
    } else if (k == RIGHT) {
        RIGHT.press();
        player_->moving = true;
        player_->direction = Direction::RIGHT; // Right
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
            player_->direction = Direction::RIGHT; // Right
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
