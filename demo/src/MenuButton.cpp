#include "MenuButton.h"

Button::Button(QPoint position, ButtonPurpose purpose)
    : purpose_(purpose) {
    setPixmap(QPixmap("images/green_block.jpg").scaled(width_, height_));
    setPos(position);
}

void Button::change_texture(const char *texture) {
    setPixmap(QPixmap(QPixmap(texture)).scaled(width_, height_)); // TODO быть может лишний QPixmap создается
}

Button::ButtonPurpose Button::purpose() const {
    return purpose_;
}
