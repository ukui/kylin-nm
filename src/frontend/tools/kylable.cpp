#include "kylable.h"
#include <QEvent>
#include <QPainter>
#include <QApplication>

#define FOREGROUND_COLOR_NORMAL qApp->palette().text().color()
#define FOREGROUND_COLOR_HOVER QColor(55,144,250,255)
#define FOREGROUND_COLOR_PRESS QColor(36,109,212,255)

KyLable::KyLable(QWidget *parent) : QLabel(parent)
{
    connect(qApp, &QApplication::paletteChanged, this, &KyLable::onPaletteChanged);
    onPaletteChanged();
}

void KyLable::onPaletteChanged()
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
    this->repaint();
}

void KyLable::paintEvent(QPaintEvent *event)
{
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::WindowText, m_foregroundColor);
    this->setPalette(pal);
    return QLabel::paintEvent(event);
}

void KyLable::enterEvent(QEvent *event)
{
    m_foregroundColor = FOREGROUND_COLOR_HOVER;
    this->update();
}

void KyLable::leaveEvent(QEvent *event)
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
    this->update();
}

void KyLable::mousePressEvent(QMouseEvent *event)
{
    m_foregroundColor = FOREGROUND_COLOR_PRESS;
    this->update();
    return QLabel::mousePressEvent(event);
}

void KyLable::mouseReleaseEvent(QMouseEvent *event)
{
    m_foregroundColor = FOREGROUND_COLOR_HOVER;
    this->update();
    return QLabel::mouseReleaseEvent(event);
}
