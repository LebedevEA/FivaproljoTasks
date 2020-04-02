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
    class Key {
    public:
        Key() = delete;
        explicit Key(Qt::Key name);
        operator Qt::Key() const;
        bool is_pressed() const;
        void press();
        void release();
	    
    private:
        Qt::Key qt_name_;
        bool is_pressed_;
    };
    
    class Manipulator {
    public:
	bool active() const;
	void activate();
	void deactivate();
	virtual void press(Qt::Key) = 0;
	virtual void release(Qt::Key) = 0;
    protected:
	bool is_active_ = false;
    };
   
    class PlayerManipulator : public Manipulator {
    public:
	PlayerManipulator() = delete;
	PlayerManipulator(Player *player, Qt::Key up_key = Qt::Key_W, Qt::Key left_key = Qt::Key_A,
                       Qt::Key down_key = Qt::Key_S, Qt::Key right_key = Qt::Key_D);
        void press(Qt::Key k) override;
        void release(Qt::Key k) override;
    private:
	Key UP, LEFT, DOWN, RIGHT;
        Player *player_;
    };
    
    std::vector<Manipulator *> manipulators_;
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
