#include "divider.h"
#include <QPainter>
#include <QApplication>

Divider::Divider(QWidget * parent) : QFrame(parent)
{
    this->setFixedHeight(1);
}



void Divider::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    QColor color = qApp->palette().color(QPalette::BrightText);
    color.setAlphaF(0.08);
    p.save();
    p.setBrush(color);
    p.setPen(Qt::transparent);
    p.drawRoundedRect(this->rect(), 6, 6);
    p.restore();
    return QFrame::paintEvent(e);
}
