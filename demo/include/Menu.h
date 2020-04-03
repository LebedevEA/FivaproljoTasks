#pragma once

#include <QGraphicsPixmapItem>
#include "Scene.h"
#include "Utilities.h"
#include "Controller.h"
//TODO mb timer

class Controller;

enum class ButtonPurpose {
    SINGLEPLAYER,
    MULTIPLAYER,
    EXIT
};

class Button : public QGraphicsPixmapItem {
public:
    Button(QPoint position, ButtonPurpose purpose);
    void change_texture(const char *texture);
    
private:
    ButtonPurpose purpose_;
    QPoint pos_;
    size_t width_ = 200;
    size_t height_ = 100;
};

class Menu : public QObject {
    Q_OBJECT
    
public:
    Menu(Scene *scene);
    void run_menu();
    void go_down();
    void go_up();
    void press();

private slots:
    void advance_menu();
    
private:
    QTimer *main_timer_ = nullptr;
    Scene *scene_;
    size_t selected_index_ = 0;
    std::vector<Button *> buttons_;
};
