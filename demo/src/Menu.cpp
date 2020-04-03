#include "Menu.h"

Button::Button(QPoint position, ButtonPurpose purpose)
    : purpose_(purpose) {
    setPixmap(QPixmap("images/green_block.jpg").scaled(width_, height_));
    setPos(position);
}

void Button::change_texture(const char *texture) {
    setPixmap(QPixmap(QPixmap(texture)).scaled(width_, height_)); // TODO быть может лишний QPixmap создается
}

Menu::Menu(Scene *scene)
    : scene_(scene) {
    buttons_.push_back(new Button({100, 200}, ButtonPurpose::SINGEPLAYER));
    buttons_.push_back(new Button({350, 200}, ButtonPurpose::SINGEPLAYER));
    // main_timer_ = new QTimer(this);
    // QObject::connect(main_timer_, SIGNAL(timeout()), this, SLOT(advance_menu()));
    // main_timer_->start(10);
    // main_timer_->setInterval(10);
}

void Menu::run_menu() {
    main_timer_ = new QTimer(this);
    QObject::connect(main_timer_, SIGNAL(timeout()), this, SLOT(advance_menu()));
    main_timer_->start(10);
    main_timer_->setInterval(10);
}

void Menu::go_down() {
    selected_index_++;
    selected_index_ %= buttons_.size();
}

void Menu::go_up() {
    selected_index_ += buttons_.size();
    selected_index_--;
    selected_index_ %= buttons_.size();
}

void Menu::press() {
    delete main_timer_;
    main_timer = nullptr;
    
}

void Menu::advance_menu() {
    for (int i = 0; i < buttons_.size(); i++) {
	if (i == selecred_index_) {
	    buttons_[i]->change_texture("images/green_bloack.jpg");
	} else {
	    buttons_[i]->change_texture("images/orange_bloack.jpg");
	}
    }	
}
