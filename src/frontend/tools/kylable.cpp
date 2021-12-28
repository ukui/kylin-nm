#include "kylable.h"
#include <QEvent>
#include <QPainter>
#include <QApplication>

#define FOREGROUND_COLOR_NORMAL qApp->palette().text().color()
#define FOREGROUND_COLOR_HOVER qApp->palette().brightText().color()
#define FOREGROUND_COLOR_PRESS qApp->palette().brightText().color()

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

void KyLable::setPressColor()
{
    QColor color = FOREGROUND_COLOR_HOVER;
    color.setAlphaF(0.2);
    m_foregroundColor = color;
}

void KyLable::setHoverColor()
{
    QColor color = FOREGROUND_COLOR_HOVER;
    color.setAlphaF(0.05);
    m_foregroundColor = color;
}

void KyLable::setNormalColor()
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
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
    setHoverColor();
    this->update();
}

void KyLable::leaveEvent(QEvent *event)
{
    setNormalColor();
    this->update();
}

void KyLable::mousePressEvent(QMouseEvent *event)
{
    setPressColor();
    this->update();
    return QLabel::mousePressEvent(event);
}

void KyLable::mouseReleaseEvent(QMouseEvent *event)
{
    setHoverColor();
    this->update();
    return QLabel::mouseReleaseEvent(event);
}
