#include "infobutton.h"
#include <QEvent>
#include <QIcon>

#define BUTTON_SIZE 16,16

InfoButton::InfoButton(QWidget *parent) : QPushButton(parent)
{
    initUI();
    installEventFilter(this);
}

void InfoButton::initUI()
{
    this->setFixedSize(BUTTON_SIZE);
    this->setText("i");
}

void InfoButton::enterEvent(QEvent *)
{
}

void InfoButton::leaveEvent(QEvent *)
{
}

bool InfoButton::eventFilter(QObject *w, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress) {
        emit this->clicked();
    }
    return QPushButton::eventFilter(w, e);
}

void InfoButton::paintEvent(QPaintEvent *event)
{
    return QPushButton::paintEvent(event);
}
