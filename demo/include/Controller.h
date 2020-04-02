#pragma once

#include <vector>
#include <QWidget>
#include <QtGui>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsItem>
#include "Player.h"
#include "Scene.h"
#include "Model.h"

class KeyPresser : public QWidget {
public:
    explicit KeyPresser(Player *player, QWidget *parent = nullptr);
    KeyPresser(Player *player1, Player *player2, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    class PlayerManipulator_ {
    public:
	PlayerManipulator_();// = delete // TODO пофиксить вместе с наследованием и закидыванием всех манипуляторов в векетор
	PlayerManipulator_(Player *player, Qt::Key up_key = Qt::Key_W, Qt::Key left_key = Qt::Key_A,
	                   Qt::Key down_key = Qt::Key_S, Qt::Key right_key = Qt::Key_D);
    	void press(Qt::Key k);
    	void release(Qt::Key k);
    private:
    	class Key_ {
    	public:
//	    Key_() = delete; TODO смотри строку 24
	    explicit Key_(Qt::Key name);
    	    operator Qt::Key() const;
    	    bool is_pressed() const;
    	    void press();
    	    void release();
	    
    	private:
    	    Qt::Key qt_name_;
    	    bool is_pressed_;
    	} UP, LEFT, DOWN, RIGHT;
        Player *player_;
	bool is_active_ = false;
    public:
	bool is_active() const;
    };
    PlayerManipulator_  player_manipulator_, second_player_manipulator;
};

class Controller {
public:
    void runGame();
private:
    std::vector<Player *> players_;
    Scene *scene_ = nullptr;
    Model *model_ = nullptr;
    KeyPresser *key_presser_ = nullptr;
};
