#include "infobutton.h"
#include <QEvent>

InfoButton::InfoButton(QWidget *parent) : QPushButton(parent)
{
    initUI();
    installEventFilter(this);
}

void InfoButton::initUI()
{

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
    }
    return QPushButton::eventFilter(w, e);
}
