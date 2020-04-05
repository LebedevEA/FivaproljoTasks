#include "Menu.h"

Menu::Menu(Scene *scene, StateMachine *state_machine)
    : scene_(scene)
    , state_machine_(state_machine)
{}

void Menu::run_menu() {
    buttons_.push_back(new Button({100, 200}, Button::ButtonPurpose::SINGLE_PLAYER));
    buttons_.push_back(new Button({350, 200}, Button::ButtonPurpose::EXIT));
    for (auto button : buttons_) {
    	scene_->add_button(button);
	qDebug() << "run_menu()";
    }
    scene_->show();
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
    if (buttons_[selected_index_]->purpose() == Button::ButtonPurpose::MULTI_PLAYER) {
	delete main_timer_;
	main_timer_ = nullptr;
	for (auto button : buttons_) {
	    delete button;
	}
	buttons_.clear();
	state_machine_->run(Utilities::GameState::MULTI_PLAYER);
    }
    // TODO other if's
}

void Menu::advance_menu() {
    for (int i = 0; i < buttons_.size(); i++) {
	if (i == selected_index_) {
	    buttons_[i]->change_texture("images/green_block.jpg");
	} else {
	    buttons_[i]->change_texture("images/orange_block.jpg");
	}
    }	
}

MenuHelper::MenuHelper(Menu *menu)
    : menu_(menu)
{}

void MenuHelper::call_run_menu() {
    menu_->run_menu();
}

std::function<void()> MenuHelper::get_run_menu() {
    return std::bind(&MenuHelper::call_run_menu, *this);
}
