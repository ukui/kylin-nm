#include "infobutton.h"
#include <QEvent>

InfoButton::InfoButton(QWidget *parent) : QPushButton(parent)
{
    initUI();
    installEventFilter(this);
}

void InfoButton::initUI()
{
    info_img = QIcon::fromTheme("network-wired-connected-symbolic", QIcon::fromTheme("network-wired-symbolic", QIcon(":/res/l/network-online.svg")));
    info_imgHover = QIcon::fromTheme("network-wireless-signal-excellent-symbolic", QIcon(":/res/x/wifi-list-bg.svg"));
    info_imgPressed = QIcon::fromTheme("document-page-setup-symbolic", QIcon(":/res/x/setup.png"));
    setIcon(info_img);
}

bool InfoButton::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::Enter:
        setIcon(info_imgHover);
        break;
    case QEvent::Leave:
        setIcon(info_img);
        break;
    case QEvent::MouseButtonPress:
        setIcon(info_imgPressed);
        break;
    case QEvent::MouseButtonRelease:
        setIcon(info_imgHover);
        break;
    default:
        break;
    }
    return QPushButton::event(event);
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
