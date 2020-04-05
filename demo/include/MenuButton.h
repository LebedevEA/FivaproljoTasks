#pragma once

#include <QGraphicsPixmapItem>
#include "Object.h"

class Button : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT;
    
public:
    enum class ButtonPurpose {
	SINGLE_PLAYER,
	MULTI_PLAYER,
	EXIT
    };
    Button(QPoint position, ButtonPurpose purpose);
    void change_texture(const char *texture);
    ButtonPurpose purpose() const;
    
private:
    QPoint pos_;
    ButtonPurpose purpose_;
    size_t width_ = 200;
    size_t height_ = 100;
};
