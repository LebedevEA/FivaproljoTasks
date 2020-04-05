#pragma once

#include <vector>
#include <functional>
#include <QGraphicsPixmapItem>
#include <QObject>
#include "Scene.h"
#include "StateMachine.h"
#include "MenuButton.h"

class Menu : public QObject {
    Q_OBJECT;
    
public:
    Menu(Scene *scene, StateMachine *state_machine);
    void run_menu();
    void go_down();
    void go_up();
    void press();

private slots:
    void advance_menu();
    
private:
    QTimer *main_timer_ = nullptr;
    Scene *scene_;
    StateMachine *state_machine_;
    size_t selected_index_ = 0;
    std::vector<Button *> buttons_;
};

class MenuHelper { // Костыль, чтобы брать у него std::function, ибо без него не работает (Спасибо Qt!).
public:
    MenuHelper(Menu *menu);
    void call_run_menu();
    std::function<void()> get_run_menu();
    
private:
    Menu *menu_;
};
