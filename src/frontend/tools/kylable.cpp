#include "kylable.h"
#include <QEvent>
#include <QPainter>
#include <QApplication>

#define FOREGROUND_COLOR_NORMAL qApp->palette().text().color()

static inline qreal mixQreal(qreal a, qreal b, qreal bias)
{
    return a + (b - a) * bias;
}


QColor mixColor(const QColor &c1, const QColor &c2, qreal bias)
{
    if (bias <= 0.0) {
        return c1;
    }
    if (bias >= 1.0) {
        return c2;
    }
    if (qIsNaN(bias)) {
        return c1;
    }

    qreal r = mixQreal(c1.redF(),   c2.redF(),   bias);
    qreal g = mixQreal(c1.greenF(), c2.greenF(), bias);
    qreal b = mixQreal(c1.blueF(),  c2.blueF(),  bias);
    qreal a = mixQreal(c1.alphaF(), c2.alphaF(), bias);

    return QColor::fromRgbF(r, g, b, a);
}

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
    QColor hightlight = this->palette().color(QPalette::Active,QPalette::Highlight);
    QColor mix = this->palette().color(QPalette::Active,QPalette::BrightText);
    m_foregroundColor = mixColor(hightlight, mix, 0.05);
}

void KyLable::setHoverColor()
{
    QColor hightlight = this->palette().color(QPalette::Active,QPalette::Highlight);
    QColor mix = this->palette().color(QPalette::Active,QPalette::BrightText);
    m_foregroundColor = mixColor(hightlight, mix, 0.2);
}

void KyLable::setNormalColor()
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
}

void KyLable::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();
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
