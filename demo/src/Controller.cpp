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

KeyPresser::KeyPresser(Player *player, QWidget *parent)
    : player_manipulator_(player) {
    setWindowOpacity(0.0);
    setFocus();
}

void KeyPresser::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        player_manipulator_.press((Qt::Key) event->key()); //TODO cast
        qDebug() << "Pressed!";
    }
}

void KeyPresser::keyReleaseEvent(QKeyEvent *event) {
    player_manipulator_.release((Qt::Key) event->key()); //TODO cast
}

KeyPresser::PlayerManipulator_::PlayerManipulator_(Player *player, Qt::Key up_key, Qt::Key left_key,
						   Qt::Key down_key, Qt::Key right_key)
    : player_(player)
    , UP(up_key)
    , LEFT(left_key)
    , DOWN(down_key)
    , RIGHT(right_key)
{}

KeyPresser::PlayerManipulator_::Key_::Key_(Qt::Key qt_name)
    : qt_name_(qt_name), is_pressed_(false) {}

KeyPresser::PlayerManipulator_::Key_::operator Qt::Key() const {
    return qt_name_;
}

bool KeyPresser::PlayerManipulator_::Key_::is_pressed() const {
    return is_pressed_;
}

void KeyPresser::PlayerManipulator_::Key_::press() {
    is_pressed_ = true;
}

void KeyPresser::PlayerManipulator_::Key_::release() {
    is_pressed_ = false;
}

void KeyPresser::PlayerManipulator_::press(Qt::Key k) {
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

void KeyPresser::PlayerManipulator_::release(Qt::Key k) {
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
