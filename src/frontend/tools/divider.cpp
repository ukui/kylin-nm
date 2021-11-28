#include "divider.h"
#include <QPainter>

Divider::Divider(QWidget * parent) : QFrame(parent)
{
    this->setFixedHeight(1);
}



void Divider::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    QColor color;
    color.setRgb(77,77,77);
    p.save();
    p.setBrush(color);
    p.setPen(Qt::transparent);
    p.setOpacity(0.3);
    p.drawRoundedRect(this->rect(), 6, 6);
    p.restore();
    return QFrame::paintEvent(e);
}
