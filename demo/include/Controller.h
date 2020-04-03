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
#include "Utilities.h"

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
        bool is_pressed_ = false;
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

    class MenuManipulator : public Manipulator {
    public:
	MenuManipulator();
	void press(Qt::Key k) override;
	void release(Qt::Key k) override;
    private:
	Key UP, DOWN;
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
    void runGame(MenuRetVal gamemod = MenuRetVal::NONE); //TODO mb change name
    std::function<void()> call_menu; // TODO сделать сеттер и 
    
private:
    bool is_menu_active = true;
    std::vector<Player *> players_;
    Menu *menu_ = nullptr;
    Scene *scene_ = nullptr;
    Model *model_ = nullptr;
    KeyPresser *key_presser_ = nullptr;
};
