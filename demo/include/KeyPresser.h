#pragma once

#include <vector>
#include <QWidget>
#include <QtGui>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsItem>
#include <QObject>
#include "Player.h"
#include "Scene.h"
#include "Model.h"
#include "Menu.h"

class KeyPresser : public QWidget {
public:
    // explicit KeyPresser(Player *player, QWidget *parent = nullptr);
    // KeyPresser(Player *player1, Player *player2, QWidget *parent = nullptr);
    explicit KeyPresser(QWidget *parent = nullptr);
    void add_players(Player *player1, Player *player2 = nullptr); // TODO later mb add overload
    void remove_players();
    void add_menu(Menu *menu);
    // remove menu is needless
    void activate(KeyPresserUtility::ManipulatorType type);
    void deactivate(KeyPresserUtility::ManipulatorType type);

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
	// TODO destructor
	bool active() const;
	void activate();
	void deactivate();
	virtual void press(Qt::Key) = 0;
	virtual void release(Qt::Key) = 0;
	KeyPresserUtility::ManipulatorType type() const;
	
    protected:
	Manipulator(bool is_active, KeyPresserUtility::ManipulatorType manip_type);
	bool is_active_;
	KeyPresserUtility::ManipulatorType manip_type_;
    };

    class MenuManipulator : public Manipulator {
    public:
	MenuManipulator(Menu *menu);
	void press(Qt::Key k) override;
	void release(Qt::Key k) override;
	
    private:
	Key UP, DOWN, RETURN;
	Menu *menu_;
    };
   
    class PlayerManipulator : public Manipulator {
    public:
	PlayerManipulator() = delete;
	PlayerManipulator(Player *player, Qt::Key up_key = Qt::Key_W, Qt::Key left_key = Qt::Key_A,
			  Qt::Key down_key = Qt::Key_S, Qt::Key right_key = Qt::Key_D); // порядок - W A S D.
        void press(Qt::Key k) override;
        void release(Qt::Key k) override;
	
    private:
	Key UP, LEFT, DOWN, RIGHT;
        Player *player_;
    };
    
    std::vector<Manipulator *> manipulators_;
};

class KeyPresserHelper {
public:
    KeyPresserHelper(KeyPresser *key_presser);
    void call_activate(KeyPresserUtility::ManipulatorType type);
    void call_deactivate(KeyPresserUtility::ManipulatorType type);
    std::function<void(KeyPresserUtility::ManipulatorType)> get_activate();
    std::function<void(KeyPresserUtility::ManipulatorType)> get_deactivate();

private:
    KeyPresser *key_presser_;
};
